#include "pch.h"
#include "ObjectManager.h"
#include <algorithm>

ObjectManager::~ObjectManager()
{
	Clear();
}

void ObjectManager::Add(Object* object)
{
	if (object == nullptr)
	{
		return;
	}

	auto findItr = std::find(_objects.begin(), _objects.end(), object);

	if (findItr == _objects.end())
	{
		_objects.push_back(object);
	}
}

void ObjectManager::Remove(Object* object)
{
	if (object == nullptr)
	{
		return;
	}

	// 하나만 지울때는 끝까지 돌지 않는 std::find-erase도 정석적이지만, 
	// 강의와 차별점 : 순서가 상관없다면 swap-pop이 최고다. 정확히는 여기서는 move-pop이지만.

	// 단, move-pop은 자신을 자신에게 move 할 경우, 원소가 하나일때는 문제가 된다.
	// 애초에 move를 안 하는 타입이거나 std 지원 컨테이너라면 방지 코드가 있어 상관 없다만,
	// 사용자 정의 클래스라면 1. 자기 자신을 넣는 것의 예방 코드를 짜던가,
	// 2. 안전하게 swap-pop을 하던가 해야한다.

	auto findItr = std::find(_objects.begin(), _objects.end(), object);

	if (findItr != _objects.end())
	{
		*findItr = std::move(_objects.back());

		_objects.pop_back();

		delete object;
	}
}

void ObjectManager::Clear()
{
	// 강의와 차별점 : 여기 [=]를 붙일 필요가 없다. 람다 '내부'에서는 +objects를 쓰지 않기 때문이다.
	std::for_each(_objects.begin(), _objects.end(), [](Object* obj) { delete obj; });

	_objects.clear();
}