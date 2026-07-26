#pragma once
#include <assert.h>

// array랑 거의 같음. 그냥 동적으로 움직이는 것만 더 있으면 됨.

template<typename T>
class VectorIterator
{
public:
	VectorIterator() : _ptr(nullptr) {}
	VectorIterator(T* ptr) : _ptr(ptr) {}

	VectorIterator& operator++ ()
	{
		++_ptr;
		return *this;
	}

	VectorIterator operator++ (int) // 후위
	{
		VectorIterator temp = *this;
		++_ptr;
		return temp;
	}

	VectorIterator operator + (const int count) const
	{
		VectorIterator temp = *this;
		temp._ptr += count;
		return temp;
	}

	bool operator == (const VectorIterator& other) const
	{
		return _ptr == other._ptr;
	}

	bool operator != (const VectorIterator& other) const
	{
		return _ptr != other._ptr;
	}

	T& operator * ()
	{
		return *_ptr;
	}
public:
	T* _ptr = nullptr;
};

template <typename T>
class Vector
{
public:
	// 굳이 외부 클래스를 Vector 클래스를 통해 이렇게 부르는 거 맞다.
	// 아니, 대체 왜요? -> 가장 핵심 : 컨테이너::VectorIterator 이 형태를 유지해야 template로 돌려 쓸 거 아니야. auto는 문제 없겠다만
	// 그리고 만약 클래스 이름을 바꿔도 은닉화해서 사용자가 코드를 안 바꿔도 되니까 + 컨테이너 이름이 VectorIterator이름에 붙어 널려있는 것보다 깔금 뭐 이건 부수적인 것
	using iterator = VectorIterator<T>;

	iterator begin() { return iterator(_buffer); }
	iterator end() { return iterator(_buffer + _size); }

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