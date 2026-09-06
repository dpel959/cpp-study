#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stack>
#include <string>
#include <vector>

namespace
{
    class LinearScanInventory
    {
    public:
        explicit LinearScanInventory(std::size_t capacity)
            : _items(capacity, nullptr)
        {
        }

        bool Add(const void* item)
        {
            for (const void*& slot : _items)
            {
                if (slot == nullptr)
                {
                    slot = item;
                    return true;
                }
            }

            return false;
        }

    private:
        std::vector<const void*> _items;
    };

    // This is an independently written model of the portfolio implementation.
    // It is not copied from lecture source code.
    class TrackedSlotInventory
    {
    public:
        explicit TrackedSlotInventory(std::size_t capacity)
            : _items(capacity, nullptr)
        {
            for (std::size_t index = 0; index < capacity; ++index)
                _emptyIndices.push(index);
        }

        bool Add(const void* item)
        {
            if (_emptyIndices.empty())
                return false;

            const std::size_t index = _emptyIndices.top();
            _emptyIndices.pop();

            _items[index] = item;
            _occupiedIndices.push_back(index);
            return true;
        }

    private:
        std::vector<const void*> _items;
        std::stack<std::size_t> _emptyIndices;
        std::vector<std::size_t> _occupiedIndices;
    };

    class FindAndEraseOccupiedSlots
    {
    public:
        FindAndEraseOccupiedSlots(std::size_t capacity, std::size_t occupiedCount)
        {
            _occupiedIndices.reserve(capacity);
            for (std::size_t index = 0; index < occupiedCount; ++index)
                _occupiedIndices.push_back(index);
        }

        bool Remove(std::size_t slotIndex)
        {
            const auto iterator = std::find(
                _occupiedIndices.begin(),
                _occupiedIndices.end(),
                slotIndex);

            if (iterator == _occupiedIndices.end())
                return false;

            _occupiedIndices.erase(iterator);
            return true;
        }

    private:
        std::vector<std::size_t> _occupiedIndices;
    };

    class SwapAndPopOccupiedSlots
    {
    public:
        SwapAndPopOccupiedSlots(std::size_t capacity, std::size_t occupiedCount)
            : _positionBySlot(capacity, InvalidPosition)
        {
            _occupiedIndices.reserve(capacity);
            for (std::size_t index = 0; index < occupiedCount; ++index)
            {
                _positionBySlot[index] = _occupiedIndices.size();
                _occupiedIndices.push_back(index);
            }
        }

        bool Remove(std::size_t slotIndex)
        {
            if (slotIndex >= _positionBySlot.size())
                return false;

            const std::size_t removePosition = _positionBySlot[slotIndex];
            if (removePosition == InvalidPosition)
                return false;

            const std::size_t lastSlotIndex = _occupiedIndices.back();
            _occupiedIndices[removePosition] = lastSlotIndex;
            _positionBySlot[lastSlotIndex] = removePosition;

            _occupiedIndices.pop_back();
            _positionBySlot[slotIndex] = InvalidPosition;
            return true;
        }

    private:
        static constexpr std::size_t InvalidPosition =
            std::numeric_limits<std::size_t>::max();

        std::vector<std::size_t> _occupiedIndices;
        std::vector<std::size_t> _positionBySlot;
    };

    class PointerLinearLookup
    {
    public:
        PointerLinearLookup(std::size_t capacity, std::size_t occupiedCount)
            : _tokens(occupiedCount), _items(capacity, nullptr)
        {
            for (std::size_t index = 0; index < occupiedCount; ++index)
                _items[index] = &_tokens[index];
        }

        const void* MakeQuery(std::size_t slotIndex) const
        {
            return &_tokens[slotIndex];
        }

        __declspec(noinline) int Find(const void* item) const
        {
            for (std::size_t index = 0; index < _items.size(); ++index)
            {
                if (_items[index] == item)
                    return static_cast<int>(index);
            }

            return -1;
        }

    private:
        std::vector<int> _tokens;
        std::vector<const void*> _items;
    };

    struct BenchmarkHandle
    {
        std::size_t slotIndex;
        std::uint32_t generation;
    };

    class GenerationalHandleLookup
    {
    public:
        GenerationalHandleLookup(std::size_t capacity, std::size_t occupiedCount)
            : _items(capacity, nullptr), _generations(capacity, 1)
        {
            for (std::size_t index = 0; index < occupiedCount; ++index)
                _items[index] = &_itemToken;
        }

        BenchmarkHandle MakeQuery(std::size_t slotIndex) const
        {
            return BenchmarkHandle{ slotIndex, _generations[slotIndex] };
        }

        __declspec(noinline) int Find(BenchmarkHandle handle) const
        {
            if (handle.slotIndex >= _items.size())
                return -1;

            if (_items[handle.slotIndex] == nullptr
                || _generations[handle.slotIndex] != handle.generation)
            {
                return -1;
            }

            return static_cast<int>(handle.slotIndex);
        }

    private:
        int _itemToken = 1;
        std::vector<const void*> _items;
        std::vector<std::uint32_t> _generations;
    };

    struct Scenario
    {
        std::size_t capacity;
        int occupancyPercent;
        std::size_t sampleCount;
    };

    template <typename InventoryType>
    double MeasureAdd(const Scenario& scenario)
    {
        static const int itemToken = 1;

        std::vector<InventoryType> inventories;
        inventories.reserve(scenario.sampleCount);

        const std::size_t occupiedCount =
            scenario.capacity * static_cast<std::size_t>(scenario.occupancyPercent) / 100;

        for (std::size_t sample = 0; sample < scenario.sampleCount; ++sample)
        {
            inventories.emplace_back(scenario.capacity);
            for (std::size_t item = 0; item < occupiedCount; ++item)
                inventories.back().Add(&itemToken);
        }

        std::size_t succeededCount = 0;
        const auto start = std::chrono::steady_clock::now();

        for (InventoryType& inventory : inventories)
            succeededCount += inventory.Add(&itemToken) ? 1U : 0U;

        const auto end = std::chrono::steady_clock::now();

        if (succeededCount != scenario.sampleCount)
            return 0.0;

        const auto elapsedNanoseconds =
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        return static_cast<double>(elapsedNanoseconds) /
            static_cast<double>(scenario.sampleCount);
    }

    template <typename InventoryType>
    double MeasureMedianAdd(const Scenario& scenario)
    {
        constexpr int RepeatCount = 5;
        std::vector<double> measurements;
        measurements.reserve(RepeatCount);

        for (int repeat = 0; repeat < RepeatCount; ++repeat)
            measurements.push_back(MeasureAdd<InventoryType>(scenario));

        std::sort(measurements.begin(), measurements.end());
        return measurements[RepeatCount / 2];
    }

    template <typename InventoryType>
    double MeasureRemove(const Scenario& scenario)
    {
        const std::size_t occupiedCount =
            scenario.capacity * static_cast<std::size_t>(scenario.occupancyPercent) / 100;
        const std::size_t targetSlot = occupiedCount / 2;

        std::vector<InventoryType> inventories;
        inventories.reserve(scenario.sampleCount);

        for (std::size_t sample = 0; sample < scenario.sampleCount; ++sample)
            inventories.emplace_back(scenario.capacity, occupiedCount);

        std::size_t succeededCount = 0;
        const auto start = std::chrono::steady_clock::now();

        for (InventoryType& inventory : inventories)
            succeededCount += inventory.Remove(targetSlot) ? 1U : 0U;

        const auto end = std::chrono::steady_clock::now();

        if (succeededCount != scenario.sampleCount)
            return 0.0;

        const auto elapsedNanoseconds =
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        return static_cast<double>(elapsedNanoseconds) /
            static_cast<double>(scenario.sampleCount);
    }

    template <typename InventoryType>
    double MeasureMedianRemove(const Scenario& scenario)
    {
        constexpr int RepeatCount = 5;
        std::vector<double> measurements;
        measurements.reserve(RepeatCount);

        for (int repeat = 0; repeat < RepeatCount; ++repeat)
            measurements.push_back(MeasureRemove<InventoryType>(scenario));

        std::sort(measurements.begin(), measurements.end());
        return measurements[RepeatCount / 2];
    }

    volatile std::size_t lookupChecksum = 0;

    template <typename InventoryType>
    double MeasureLookup(const Scenario& scenario)
    {
        const std::size_t occupiedCount =
            scenario.capacity * static_cast<std::size_t>(scenario.occupancyPercent) / 100;
        InventoryType inventory(scenario.capacity, occupiedCount);

        std::vector<std::size_t> querySlots;
        querySlots.reserve(scenario.sampleCount);

        std::uint32_t state = 0x12345678U;
        for (std::size_t sample = 0; sample < scenario.sampleCount; ++sample)
        {
            state = state * 1664525U + 1013904223U;
            querySlots.push_back(static_cast<std::size_t>(state) % occupiedCount);
        }

        std::size_t checksum = 0;
        const auto start = std::chrono::steady_clock::now();

        for (std::size_t slotIndex : querySlots)
        {
            const auto query = inventory.MakeQuery(slotIndex);
            checksum += static_cast<std::size_t>(inventory.Find(query));
        }

        const auto end = std::chrono::steady_clock::now();
        lookupChecksum = checksum;

        const auto elapsedNanoseconds =
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        return static_cast<double>(elapsedNanoseconds) /
            static_cast<double>(scenario.sampleCount);
    }

    template <typename InventoryType>
    double MeasureMedianLookup(const Scenario& scenario)
    {
        constexpr int RepeatCount = 5;
        std::vector<double> measurements;
        measurements.reserve(RepeatCount);

        for (int repeat = 0; repeat < RepeatCount; ++repeat)
            measurements.push_back(MeasureLookup<InventoryType>(scenario));

        std::sort(measurements.begin(), measurements.end());
        return measurements[RepeatCount / 2];
    }
}

int main()
{
    const std::vector<Scenario> scenarios{
        { 100, 50, 5000 },
        { 100, 90, 5000 },
        { 100, 99, 5000 },
        { 1000, 90, 500 },
        { 1000, 99, 500 },
        { 10000, 90, 50 },
        { 10000, 99, 50 },
    };

    const std::vector<Scenario> removalScenarios{
        { 100, 50, 20000 },
        { 100, 90, 20000 },
        { 100, 99, 20000 },
        { 1000, 90, 2000 },
        { 1000, 99, 2000 },
        { 10000, 90, 200 },
        { 10000, 99, 200 },
    };

    const std::vector<Scenario> lookupScenarios{
        { 100, 50, 1000000 },
        { 100, 90, 1000000 },
        { 100, 99, 1000000 },
        { 1000, 90, 200000 },
        { 1000, 99, 200000 },
        { 10000, 90, 20000 },
        { 10000, 99, 20000 },
    };

    std::cout << "Add operation benchmark (lower is better)\n";
    std::cout << "Run the Release x64 build without a debugger.\n\n";
    std::cout << "Each value is the median of five runs. "
              << "A ratio above 1.0 means tracked slots were faster.\n\n";
    std::cout << std::left
              << std::setw(12) << "Capacity"
              << std::setw(12) << "Occupancy"
              << std::setw(20) << "Linear scan ns/op"
              << std::setw(20) << "Tracked slot ns/op"
              << "Ratio\n";

    for (const Scenario& scenario : scenarios)
    {
        const double linearNanoseconds = MeasureMedianAdd<LinearScanInventory>(scenario);
        const double trackedNanoseconds = MeasureMedianAdd<TrackedSlotInventory>(scenario);
        const double ratio = trackedNanoseconds > 0.0
            ? linearNanoseconds / trackedNanoseconds
            : 0.0;

        std::cout << std::fixed << std::setprecision(2)
                  << std::setw(12) << scenario.capacity
                  << std::setw(12) << (std::to_string(scenario.occupancyPercent) + "%")
                  << std::setw(20) << linearNanoseconds
                  << std::setw(20) << trackedNanoseconds
                  << ratio << "x\n";
    }

    std::cout << "\nSingle removal benchmark (middle occupied slot, lower is better)\n";
    std::cout << "A ratio above 1.0 means swap-and-pop was faster.\n\n";
    std::cout << std::left
              << std::setw(12) << "Capacity"
              << std::setw(12) << "Occupancy"
              << std::setw(22) << "Find + erase ns/op"
              << std::setw(22) << "Swap-and-pop ns/op"
              << "Ratio\n";

    for (const Scenario& scenario : removalScenarios)
    {
        const double eraseNanoseconds =
            MeasureMedianRemove<FindAndEraseOccupiedSlots>(scenario);
        const double swapAndPopNanoseconds =
            MeasureMedianRemove<SwapAndPopOccupiedSlots>(scenario);
        const double ratio = swapAndPopNanoseconds > 0.0
            ? eraseNanoseconds / swapAndPopNanoseconds
            : 0.0;

        std::cout << std::fixed << std::setprecision(2)
                  << std::setw(12) << scenario.capacity
                  << std::setw(12) << (std::to_string(scenario.occupancyPercent) + "%")
                  << std::setw(22) << eraseNanoseconds
                  << std::setw(22) << swapAndPopNanoseconds
                  << ratio << "x\n";
    }

    std::cout << "\nItem lookup benchmark (lower is better)\n";
    std::cout << "A ratio above 1.0 means the generational handle was faster.\n\n";
    std::cout << std::left
              << std::setw(12) << "Capacity"
              << std::setw(12) << "Occupancy"
              << std::setw(24) << "Pointer scan ns/op"
              << std::setw(24) << "Handle lookup ns/op"
              << "Ratio\n";

    for (const Scenario& scenario : lookupScenarios)
    {
        const double pointerScanNanoseconds =
            MeasureMedianLookup<PointerLinearLookup>(scenario);
        const double handleLookupNanoseconds =
            MeasureMedianLookup<GenerationalHandleLookup>(scenario);
        const double ratio = handleLookupNanoseconds > 0.0
            ? pointerScanNanoseconds / handleLookupNanoseconds
            : 0.0;

        std::cout << std::fixed << std::setprecision(2)
                  << std::setw(12) << scenario.capacity
                  << std::setw(12) << (std::to_string(scenario.occupancyPercent) + "%")
                  << std::setw(24) << pointerScanNanoseconds
                  << std::setw(24) << handleLookupNanoseconds
                  << ratio << "x\n";
    }
}
