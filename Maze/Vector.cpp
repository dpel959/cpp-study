#include "pch.h"
#include "Vector.h"
#include <type_traits>

template<typename T>
Vector<T>::Vector()
{
}

template<typename T>
Vector<T>::Vector(int capacity) : _capacity(capacity)
{
	assert(capacity > 0);

	_buffer = new T[capacity];
}

template<typename T>
Vector<T>::~Vector()
{
	if(_buffer != nullptr)
		delete[] _buffer;
}

template<typename T>
T& Vector<T>::operator[](int index) {
	assert(index >= 0 && index < _size);

	return _buffer[index];
}

template<typename T>
T& Vector<T>::back()
{
	return _buffer[_size - 1];
}

template<typename T>
void Vector<T>::push_back(const T& data)
{
	if (_size == _capacity)
	{
		if (_capacity <= 1)
			reserve(2);
		else
			reserve(_capacity * 1.5);
	}

	_buffer[_size] = data;
	++_size;
}

template<typename T>
void Vector<T>::pop_back()
{
	assert(_size > 0);

	--_size;
}

template<typename T>
void Vector<T>::reserve(int capacity)
{
	if (_capacity >= capacity)
		return;

	T* newBuffer;

	if (_capacity == 0)
		newBuffer = new T[capacity];
	else
	{
		newBuffer = new T[capacity];

		for (int i = 0; i < _size; ++i)
		{
			newBuffer[i] = std::move(_buffer[i]);
		}

		delete[] _buffer;
	}

	_capacity = capacity;
	_buffer = newBuffer;
}

// 굳이 buffer을 delete 해줄 필요는 없다.
// 다만, 원래는 객체면 순회하며 소멸자 호출해주는 과정이 필요하다.

// 기본 타입에 소멸자를 붙여도 아무런 행동도 하지 않기에, 사실 그렇게 해도 된다만
// 그건 성능 낭비이다. 제대로 소멸자가 있는지 확인하고 그것에만 해주는 것이 좋다.
// T는 컴파일 타임에 결정되므로 constexpr을 써준다.
	//if constexpr (!std::is_trivially_destructible_v<T>)
	//{
	//	for (int i = 0; i < _size; ++i)
	//	{
	//		_buffer[i].~T();
	//	}
	//}
//이런 형식으로. 지금 넣으면 Double Free 에러가 나므로 참고 삼아. 

template<typename T>
void Vector<T>::clear()
{
	_size = 0;
}

template<typename T>
int Vector<T>::size() const
{
	return _size;
}

template<typename T>
int Vector<T>::capacity() const
{
	return _capacity;
}

template<typename T>
void Vector<T>::PrintAllElements()
{
	for (int i = 0; i < _size; ++i)
	{
		cout << _buffer[i] << ' ';
	}
	cout << '\n';
}

/// <summary>
/// 템플릿 함수는 컴파일러가 보통 만드므로, 이거 중복 정의 에러내지 마세요
/// 하려면 inline 해줘야 함
/// </summary>

template<>
void Vector<Pos>::PrintAllElements()
{
	for (int i = 0; i < _size; ++i)
	{
		cout << _buffer[i].x << ' ' << _buffer[i].y << ' ';
	}
	cout << '\n';
}

template class Vector<int>;
template class Vector<Pos>;