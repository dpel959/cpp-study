#pragma once

class Object;

//얘는 그냥 데이터 들고 있는 거라 실시간 업데이트 느낌은 아님
//그래서 Init, Update, Render은 따로 없음
class ObjectManager
{
	DECLARE_SINGLE(ObjectManager);

public:
	~ObjectManager();


	void Add(Object* object);
	void Remove(Object* object);
	void Clear();

	const std::vector<Object*>& GetObjects() { return _objects; }

	// 근데 이 template은 Init이라는 함수가 T에 있는 걸 어떻게 알까?
	// 사실 아는 게 아니고, 일단 그냥 해보고 만들어 보는거다. 안 되면 컴파일에서 뱉는다.

	// 아니.. 그러면 Init이란 함수가 있는데 내가 원하지 않는 거면 어떡해요?

	template<typename T>
	T* CreateObject()
	{
		// 넣어준 T가 Object 타입으로 T가 변환이 가능하면 (즉 상속 관계면) true! 아니면 false!
		static_assert(std::is_convertible_v<T*, Object*>);

		T* object = new T();
		object->Init();

		return object;
	}

private:
	std::vector<Object*> _objects;
};