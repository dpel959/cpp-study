#include "Array.h"
#include <iostream>

template <typename T, int N>
Array<T, N>::Array()
{
}

template <typename T, int N>
Array<T, N>::~Array()
{
}

template <typename T, int N>
void Array<T, N>::push_back(const T& data)
{
	assert(_size < N);

	_buffer[_size] = data;
	++_size;
}

template <typename T, int N>
T& Array<T, N>::operator[](int index)
{
	assert(index >= 0 && index < _size); // 해당 조건이 아니면 크래시

	return _buffer[index];
}

template <typename T, int N>
int Array<T, N>::size() const
{
	return _size;
}

template<typename T, int N>
void Array<T, N>::PrintAllElements()
{
	for (int i = 0; i < _size; ++i)
	{
		std::cout << _buffer[i] << ' ';
	}
	std::cout << '\n';
}