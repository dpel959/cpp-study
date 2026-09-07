#pragma once
#include "Object.h"
#include <memory>
#include <type_traits>

class ObjectManager
{
	DECLARE_SINGLE(ObjectManager);

public:
	~ObjectManager();

	void Update();
	void Render(HDC hdc);
	void Remove(Object* object);
	void Clear();

	std::vector<Object*> GetObjects() const;
	bool IsAlive(const Object* object) const;

	// 근데 이 template은 Init이라는 함수가 T에 있는 걸 어떻게 알까?
	// 사실 아는 게 아니고, 일단 그냥 해보고 만들어 보는거다. 안 되면 컴파일에서 뱉는다.

	// 아니.. 그러면 Init이란 함수가 있는데 내가 원하지 않는 거면 어떡해요?

	template<typename T>
	T* CreateObject()
	{
		// 넣어준 T가 Object를 상속받은 타입인지 컴파일 단계에서 확인한다.
		static_assert(std::is_base_of_v<Object, T>);

		std::unique_ptr<T> object = std::make_unique<T>();
		T* objectPtr = object.get();
		objectPtr->Init();

		if (_isUpdating)
		{
			_pendingAddQ.push_back(std::move(object));
		}
		else
		{
			_objects.push_back(std::move(object));
		}

		return objectPtr;
	}

private:
	void FlushPendingObjects();
	bool IsPendingRemove(const Object* object) const;

private:
	std::vector<std::unique_ptr<Object>> _objects;

	// 강의와 차별점 : Update 도중 컨테이너를 변경하지 않고, 순회가 끝난 뒤 생성과 삭제를 반영한다.
	std::vector<std::unique_ptr<Object>> _pendingAddQ;
	std::vector<Object*> _pendingRemoveQ;
	bool _isUpdating = false;
};
