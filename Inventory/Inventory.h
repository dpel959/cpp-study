#pragma once
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
// 메모리와 성능의 trade-off 지만, 성능 쪽으로 훨씬 이득이라 판단.
// 
// 빈 곳은 stack. 어차피 빈 곳만 바로 빼와주면 됨. 딱 적합함.
// 차지한 곳은 vector. RandomRemoveItem()이 랜덤하게 아이템을 빼오는데, 
// list로 구현해서 하나하나 지우고 다시 잇는 것보다 remove로 한번에 더블포인터 작업으로 처리하고, erase 하는 게 빠르다.
// 
// SORT는 구현하지 않는다. 너무 주제에서 벗어나기도하고, 인벤토리 구현 상 굳이 SORT를 자동화하거나 넣어주지 않는 것이 더 유연하고 빠르다고 판단했다.
// *************************

class Inventory
{
public:
	bool AddItem(Item* item); // 모두 유니크 아이템이라 가정한다.
	bool RemoveItem(Item* item);
	bool RemoveItem(std::vector<int>& vec);
	int GetItemCount();
	bool RandomRemoveItem();
	Item* FindItemByIndex(int idx);
	static Inventory& Getinstance();
private:
	int FindEmptySlot();
	int FindItemSlot(Item* item);
private:
	Item* _items[MAX_SLOT]; // 이 틀은 지킨다. 여기서부터 개조
	int _itemCount;
	std::stack<int> emptyIdxStack;
	std::vector<int> occupiedIdxVec;
	Inventory();
	~Inventory();
};