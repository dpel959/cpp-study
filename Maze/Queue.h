#pragma once
#include "Vector.h"

template<typename T>
class Queue
{
public:
	Queue()
	{
		_container.resize(100);
	}

	void push(const T& value)
	{
		if (full()) // 원래 Vector의 push_back이나 reserve를 쓰면 늘려주는데, 얘는 안 하는 걸로
			return;

		_container[_back] = value;
		_back = (_back + 1) % _container.size();
		++_size;
	}

	void pop()
	{
		_front = (_front + 1) % _container.size();
		--_size;
	}

	T& front()
	{
		return _container[_front];
	}

	bool empty() { return _size == 0; }

	bool full() { return _size == _container.size(); }

private:
	Vector<T> _container;

	int _front = 0;
	int _back = 0;
	int _size = 0;
};

///
/// 효율성을 위해 벡터를 이용해야하므로, 투 포인터 기법으로 해주자.
/// 꺼내는 위치, 넣는 위치를 따로 관리한다.
///