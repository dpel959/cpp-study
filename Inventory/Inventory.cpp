#include "Inventory.h"
#include "Random.h"
#include "Item.h"
#include <algorithm>
#include <iostream>

Inventory::Inventory()
	: _items{}, _itemCount(0)
{
	for (int i = 0; i < MAX_SLOT; ++i)
	{
		emptyIdxStack.push(i);
		occupiedPosition[i] = -1;
		generations[i] = 1;
	}
}

Inventory::~Inventory() = default;

std::optional<ItemHandle> Inventory::AddItem(std::unique_ptr<Item> item)
{
	if (item == nullptr)
		return std::nullopt;
	int idx = FindEmptySlot();
	if (idx < 0)
		return std::nullopt;
	occupiedIdxVec.push_back(idx);
	occupiedPosition[idx] = static_cast<int>(occupiedIdxVec.size()) - 1;
	_items[idx] = std::move(item);
	++_itemCount;
	emptyIdxStack.pop();
	std::cout << "Add Item :\n";
	_items[idx]->PrintInfo();
	return ItemHandle{ idx, generations[idx] };
}

bool Inventory::RemoveItem(ItemHandle handle)
{
	if (!IsValidHandle(handle))
		return false;

	const int itemIdx = handle.slotIndex;

	std::cout << "Removed item :\n";
	_items[itemIdx]->PrintInfo();

	_items[itemIdx].reset();
	--_itemCount;

	RemoveOccupiedIndex(itemIdx);
	emptyIdxStack.push(itemIdx);
	AdvanceGeneration(itemIdx);

	return true;
}

bool Inventory::RemoveItems(const std::vector<ItemHandle>& handles)
{
	if (handles.empty())
		return false;

	int removedCount = 0;

	std::cout << "Removed Items:\n";

	for (ItemHandle handle : handles)
	{
		if (!IsValidHandle(handle))
			continue;

		const int idx = handle.slotIndex;

		_items[idx]->PrintInfo();
		_items[idx].reset();

		emptyIdxStack.push(idx);
		RemoveOccupiedIndex(idx);
		AdvanceGeneration(idx);

		--_itemCount;
		++removedCount;
	}

	return removedCount > 0;
}

int Inventory::GetItemCount() const
{
	return _itemCount;
}

bool Inventory::RandomRemoveItem()
{
	if (_itemCount <= 0)
		return false;

	const int removeCount = std::max(1, _itemCount / 5); // 적어도 하나는 드랍하도록

	std::vector<int> selectedIndices;
	Random::GetSample(occupiedIdxVec, removeCount, selectedIndices);

	std::vector<ItemHandle> selectedHandles;
	selectedHandles.reserve(selectedIndices.size());
	for (int idx : selectedIndices)
		selectedHandles.push_back(ItemHandle{ idx, generations[idx] });

	return RemoveItems(selectedHandles);
}

Item* Inventory::FindItem(ItemHandle handle)
{
	if (!IsValidHandle(handle))
		return nullptr;

	return _items[handle.slotIndex].get();
}

const Item* Inventory::FindItem(ItemHandle handle) const
{
	if (!IsValidHandle(handle))
		return nullptr;

	return _items[handle.slotIndex].get();
}


// *************************
// 함수 내부 static 객체의 초기화는 C++11부터 멀티 스레드 환경에서도 한 번만 수행된다.
// 단, AddItem과 RemoveItem 같은 Inventory의 멤버 함수 자체가 스레드 안전한 것은 아니다.
// *************************

Inventory& Inventory::Getinstance()
{
	static Inventory instance;

	return instance;
}

int Inventory::FindEmptySlot()
{
	if (emptyIdxStack.empty())
		return -1;
	int num = emptyIdxStack.top();
	return num;
}

void Inventory::RemoveOccupiedIndex(int itemIdx)
{
	const int removePosition = occupiedPosition[itemIdx];
	if (removePosition < 0)
		return;

	const int lastSlotIndex = occupiedIdxVec.back();
	occupiedIdxVec[removePosition] = lastSlotIndex;
	occupiedPosition[lastSlotIndex] = removePosition;

	occupiedIdxVec.pop_back();
	occupiedPosition[itemIdx] = -1;
}

bool Inventory::IsValidHandle(ItemHandle handle) const
{
	if (handle.slotIndex < 0 || handle.slotIndex >= MAX_SLOT)
		return false;

	return _items[handle.slotIndex] != nullptr
		&& generations[handle.slotIndex] == handle.generation;
}

void Inventory::AdvanceGeneration(int itemIdx)
{
	++generations[itemIdx];
	if (generations[itemIdx] == 0)
		++generations[itemIdx];
}
