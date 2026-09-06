#pragma once
#include "ItemHandle.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <stack>
#include <vector>
const int MAX_SLOT = 100;

// *************************
// 전방선언. 헤더 꼬임과 컴파일 시간 단축을 위해 필요.
// 물론 기능이 필요하다면 include 필수
// *************************

class Item;

// *************************
// 기존 강의 코드에서, Item* _items[MAX_SLOT] 이라는 모래주머니를 달고 코드를 개조한다.
// 브루트포스로 배열을 돌면서 빈 곳, 차 있는 곳을 찾는 게 아닌 stack와 vector로 차지한 곳, 아닌 곳을 관리한다.
// 추가 메모리를 사용하는 대신 빈 슬롯 탐색 비용을 줄이는 방식이다.
// 실제 성능상의 이득은 슬롯 수와 점유율에 따라 달라지므로 InventoryBenchmark에서 비교한다.
// 
// 빈 곳은 stack. 어차피 빈 곳만 바로 빼와주면 됨. 딱 적합함.
// 차지한 곳은 vector. RandomRemoveItem()이 실제 아이템이 있는 슬롯만 대상으로 표본을 뽑을 수 있다.
// vector 내부 순서는 사용하지 않으므로 역방향 위치표와 swap-and-pop을 이용해 점유 슬롯을 O(1)에 제거한다.
// 
// SORT는 구현하지 않는다. 너무 주제에서 벗어나기도하고, 인벤토리 구현 상 굳이? SORT를 자동화하거나 넣어주지 않는 것이 더 유연하고 빠르다고 판단했다.
// *************************

class Inventory
{
public:
	std::optional<ItemHandle> AddItem(std::unique_ptr<Item> item); // 아이템의 소유권을 Inventory로 이전한다.
	bool RemoveItem(ItemHandle handle);
	bool RemoveItems(const std::vector<ItemHandle>& handles);
	int GetItemCount() const;
	bool RandomRemoveItem();
	Item* FindItem(ItemHandle handle);
	const Item* FindItem(ItemHandle handle) const;
	static Inventory& Getinstance();
private:
	int FindEmptySlot();
	void RemoveOccupiedIndex(int itemIdx);
	bool IsValidHandle(ItemHandle handle) const;
	void AdvanceGeneration(int itemIdx);
private:

	// 강의와 차별점: 생포인터가 아닌, 안전한 unique_ptr을 사용했다.
	// 그리고 인벤토리의 특성상, 인벤토리나, 필드나, 장비 칸이나, 우편함이나 한 객체만 소유권으로 가지고 있는 것이 적합하다고 판단했다.
	// 만약 다른 객체가 서로 item을 가리키고, item을 사용해서 해제했는데 다른 객체가 item을 가리켜 사용한 아이템이 남아있다면, 잘못된 동작일 것이기에.
	std::unique_ptr<Item> _items[MAX_SLOT];
	int _itemCount;
	std::stack<int> emptyIdxStack;
	std::vector<int> occupiedIdxVec;

	// swap-and-pop을 위해서 추가했다. occupiedIdxVec만큼의 메모리를 차지하지만, 삭제 시의 수행 시간을 O(1)로 줄일 수 있다!
	int occupiedPosition[MAX_SLOT];

	// 각 칸이 몇 번째로 사용되고 있는지를 저장한다. 아이템이 삭제될 때마다 해당 칸의 번호가 바뀐다.
	// N번째 칸이 몇 번째 사용됐는지를 저장한다. ex) [인덱스]번째 칸이 generation+1번째 사용되었다면, generation번째 사용한 아이템과 다른 아이템이다!
	// 즉, ItemHandle의 번호와 여기의 번호가 다르면, 이미 삭제된 예전 아이템을 가리키고 있다는 것.
	// 이를 통해 조회 또한 O(1) 로 바꿀 수 있었다. 단, 당연히 메모리와 trade-off 인 구조이다.
	std::uint32_t generations[MAX_SLOT];
	Inventory();
	~Inventory();
};
