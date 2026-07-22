#pragma once
#include <assert.h>

// array랑 거의 같음. 그냥 동적으로 움직이는 것만 더 있으면 됨.

template <typename T>
class Vector
{
public:

	explicit Vector();
	explicit Vector(int capacity);

	~Vector();

	T& operator[](int index);

	T& back();

	void push_back(const T& data);

	void pop_back();

	void resize(int size);

	void reserve(int capacity);
	
	void clear();

	int size() const;
	int capacity() const;

	void PrintAllElements();

private:
	T* _buffer = nullptr;
	int _size = 0;
	int _capacity = 0;
};