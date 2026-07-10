#include "Inventory.h"
#include "Random.h"
#include "Item.h"
#include <algorithm>
#include <iostream>

Inventory::Inventory()
	: _items{}, _itemCount(0)
{
	for (int i = 0; i < MAX_SLOT; ++i)
		emptyIdxStack.push(i);
}

Inventory::~Inventory()
{
	for (int idx : occupiedIdxVec)
		delete _items[idx];
}

bool Inventory::AddItem(Item* item)
{
	if (item == nullptr)
		return false;
	int idx = FindEmptySlot();
	if (idx < 0)
		return false;
	_items[idx] = item;
	++_itemCount;
	occupiedIdxVec.push_back(idx);
	emptyIdxStack.pop();
	std::cout << "Add Item :\n";
	item->PrintInfo();
	return true;
}

bool Inventory::RemoveItem(Item* item)
{
	if (item == nullptr)
		return false;

	int itemIdx = FindItemSlot(item);

	if (itemIdx < 0)
		return false;

	std::cout << "Removed item :\n";
	item->PrintInfo();

	delete _items[itemIdx];
	--_itemCount;
	int occupiedIdxVecSize = occupiedIdxVec.size();
	for (int i = 0; i < occupiedIdxVecSize; ++i)
	{
		if (occupiedIdxVec[i] == itemIdx)
		{
			occupiedIdxVec.erase(occupiedIdxVec.begin() + i);
			break;
		}
	}
	emptyIdxStack.push(itemIdx);
	_items[itemIdx] = nullptr;

	return true;
}

bool Inventory::RemoveItem(std::vector<int>& vec)
{
	if (vec.empty())
		return false;

	bool occupiedTable[MAX_SLOT] = {};

	std::cout << "Removed Items:\n";

	for (int idx : vec)
	{
		if (idx < 0 || idx >= MAX_SLOT || _items[idx] == nullptr)
			continue;

		_items[idx]->PrintInfo();
		delete _items[idx];
		_items[idx] = nullptr;

		emptyIdxStack.push(idx);

		--_itemCount;
	}

	for (int idx : vec)
		occupiedTable[idx] = true;

	occupiedIdxVec.erase(std::remove_if(occupiedIdxVec.begin(), occupiedIdxVec.end(), [&occupiedTable](int n) { return occupiedTable[n]; })
		, occupiedIdxVec.end());

	return true;
}

int Inventory::GetItemCount()
{
	return _itemCount;
}

bool Inventory::RandomRemoveItem()
{
	if (_itemCount <= 0)
		return false;

	std::vector<int> selectedVec;
	Random::GetSample(occupiedIdxVec, _itemCount / 5, selectedVec);

	return RemoveItem(selectedVec);
}

Item* Inventory::FindItemByIndex(int idx)
{
	if (idx < 0 || idx >= MAX_SLOT)
		return nullptr;

	return _items[idx];
}


// *************************
// 여기도 포인터로 하지 않고 객체로 한다. 포인터로 하면 컴파일러가 보장해주는 멀티 스레드 안전을 잃을 수 있다.
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

int Inventory::FindItemSlot(Item* item)
{
	for (int i = 0; i < MAX_SLOT; ++i)
		if (_items[i] == item)
			return i;
	return -1;
}
