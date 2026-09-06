#include "Inventory.h"
#include "Item.h"

#include <iostream>
#include <memory>
#include <streambuf>
#include <string>
#include <utility>
#include <vector>

namespace
{
    class NullBuffer final : public std::streambuf
    {
    protected:
        int overflow(int character) override
        {
            return character;
        }
    };

    class TestResult
    {
    public:
        void Check(bool condition, const std::string& name)
        {
            if (condition)
            {
                ++_passedCount;
                return;
            }

            ++_failedCount;
            _failedTests.push_back(name);
        }

        int Print() const
        {
            std::cout << "Inventory boundary tests\n";
            std::cout << "Passed: " << _passedCount << '\n';
            std::cout << "Failed: " << _failedCount << '\n';

            for (const std::string& name : _failedTests)
                std::cout << "  - " << name << '\n';

            return _failedCount == 0 ? 0 : 1;
        }

    private:
        int _passedCount = 0;
        int _failedCount = 0;
        std::vector<std::string> _failedTests;
    };

    class TrackedItem final : public Item
    {
    public:
        explicit TrackedItem(int& destructionCount)
            : Item(EItemType::Consumable), _destructionCount(destructionCount)
        {
        }

        ~TrackedItem() override
        {
            ++_destructionCount;
        }

        void PrintInfo() override
        {
        }

    private:
        int& _destructionCount;
    };
}

int main()
{
    TestResult result;
    Inventory& inventory = Inventory::Getinstance();

    NullBuffer nullBuffer;
    std::streambuf* originalBuffer = std::cout.rdbuf(&nullBuffer);

    result.Check(inventory.GetItemCount() == 0, "The inventory starts empty");
    result.Check(!inventory.AddItem(nullptr).has_value(), "A null item is rejected");

    const ItemHandle negativeIndexHandle{ -1, 1 };
    const ItemHandle outOfRangeHandle{ MAX_SLOT, 1 };
    result.Check(inventory.FindItem(negativeIndexHandle) == nullptr, "A negative handle index is rejected");
    result.Check(inventory.FindItem(outOfRangeHandle) == nullptr, "An out-of-range handle index is rejected");

    const std::vector<ItemHandle> invalidHandles{ negativeIndexHandle, outOfRangeHandle };
    result.Check(!inventory.RemoveItems(invalidHandles), "Invalid removal handles are rejected safely");

    int destructionCount = 0;
    auto trackedItem = std::make_unique<TrackedItem>(destructionCount);
    const std::optional<ItemHandle> trackedHandle = inventory.AddItem(std::move(trackedItem));
    result.Check(trackedHandle.has_value(), "A unique item can be added");
    result.Check(trackedItem == nullptr, "Ownership moves from the caller to the inventory");
    result.Check(destructionCount == 0, "A stored item remains alive");
    result.Check(
        trackedHandle.has_value() && inventory.FindItem(*trackedHandle) != nullptr,
        "A valid handle finds its item without a linear search");
    result.Check(
        trackedHandle.has_value() && inventory.RemoveItem(*trackedHandle),
        "A valid handle removes its item");
    result.Check(destructionCount == 1, "A removed item is destroyed exactly once");
    result.Check(
        trackedHandle.has_value() && inventory.FindItem(*trackedHandle) == nullptr,
        "A handle becomes stale after deletion");
    result.Check(
        trackedHandle.has_value() && !inventory.RemoveItem(*trackedHandle),
        "A stale handle cannot delete another item");

    result.Check(inventory.AddItem(std::make_unique<Weapon>()).has_value(), "One item can be added");
    result.Check(inventory.GetItemCount() == 1, "The count becomes one after adding");
    result.Check(inventory.RandomRemoveItem(), "Random removal removes at least one item");
    result.Check(inventory.GetItemCount() == 0, "The last item was removed");

    bool filledAllSlots = true;
    std::vector<ItemHandle> filledHandles;
    filledHandles.reserve(MAX_SLOT);
    for (int i = 0; i < MAX_SLOT; ++i)
    {
        auto item = std::make_unique<Weapon>();
        const std::optional<ItemHandle> handle = inventory.AddItem(std::move(item));
        if (!handle.has_value())
        {
            filledAllSlots = false;
            break;
        }

        filledHandles.push_back(*handle);
    }

    result.Check(filledAllSlots, "All inventory slots can be filled");
    result.Check(inventory.GetItemCount() == MAX_SLOT, "The count reaches MAX_SLOT");

    const bool overflowAdded = inventory.AddItem(std::make_unique<Weapon>()).has_value();
    result.Check(!overflowAdded, "An item beyond capacity is rejected");

    int rejectedItemDestructionCount = 0;
    const bool trackedOverflowAdded = inventory.AddItem(
        std::make_unique<TrackedItem>(rejectedItemDestructionCount)).has_value();
    result.Check(!trackedOverflowAdded, "Ownership-safe insertion still rejects an item beyond capacity");
    result.Check(
        rejectedItemDestructionCount == 1,
        "An item rejected by a full inventory is destroyed automatically");

    const int countBeforeDuplicateRemoval = inventory.GetItemCount();
    const ItemHandle removedHandle = filledHandles.front();
    const std::vector<ItemHandle> duplicateAndInvalidHandles{
        removedHandle,
        removedHandle,
        negativeIndexHandle,
        outOfRangeHandle
    };
    result.Check(inventory.RemoveItems(duplicateAndInvalidHandles), "A valid handle is removed among invalid inputs");
    result.Check(
        inventory.GetItemCount() == countBeforeDuplicateRemoval - 1,
        "A duplicate index removes an item only once");

    const std::optional<ItemHandle> replacementHandle = inventory.AddItem(std::make_unique<Armor>());
    result.Check(replacementHandle.has_value(), "A freed slot can be reused");
    result.Check(
        replacementHandle.has_value() && replacementHandle->slotIndex == removedHandle.slotIndex,
        "A deleted slot is reused");
    result.Check(
        replacementHandle.has_value() && replacementHandle->generation != removedHandle.generation,
        "A reused slot receives a new generation");
    result.Check(inventory.FindItem(removedHandle) == nullptr, "The old handle cannot access the replacement item");
    result.Check(inventory.GetItemCount() == MAX_SLOT, "The count returns to MAX_SLOT");

    const ItemHandle middleHandle = filledHandles[MAX_SLOT / 2];
    result.Check(inventory.RemoveItem(middleHandle), "A middle occupied item can be removed by handle");
    result.Check(
        replacementHandle.has_value() && inventory.RemoveItem(*replacementHandle),
        "The item moved by swap-and-pop remains removable by handle");
    result.Check(inventory.GetItemCount() == MAX_SLOT - 2, "Two swap-and-pop removals update the count");

    const bool firstRestoreAdded = inventory.AddItem(std::make_unique<Weapon>()).has_value();
    result.Check(firstRestoreAdded, "The first swap-and-pop slot can be reused");

    const bool secondRestoreAdded = inventory.AddItem(std::make_unique<Armor>()).has_value();
    result.Check(secondRestoreAdded, "The second swap-and-pop slot can be reused");

    result.Check(inventory.GetItemCount() == MAX_SLOT, "The inventory remains consistent after swap-and-pop");

    std::cout.rdbuf(originalBuffer);
    return result.Print();
}
