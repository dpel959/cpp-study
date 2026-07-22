#pragma once
#include "Vector.h"

/// <summary>
/// 한줄~ 두줄 같이 단순한 경우는 헤더에 정의하자.
/// 인라이닝 자동 최적화는 한 cpp 파일 안에서만 된다.
/// </summary>

template<typename T>
class Stack
{
public:
	void push(const T& value)
	{
		_container.push_back(value);
	}


	void pop()
	{
		_container.pop_back();
	}

	T& top()
	{
		return _container.back();
	}

	bool empty() { return size() > 0; };
	int size() { return _container.size(); };
private:
	Vector<T> _container;
};