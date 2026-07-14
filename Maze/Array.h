#pragma once
#include <assert.h>

// 생성자를 만들때는 explicit을 넣어주는 게 좋다.
// 그래야 외부에서 사용할때 파라미터 넘길 때 암시적 변환이 막힘.

// size는 실제 쓰는 원소, capacity는 메모리만 있고 실제로 아직 쓰지 않은 곳
// (정확히는 뒤에서 관리해주는 것이다만)

// Array는 Vector와 다르게 heap 메모리가 아닌 stack 메모리에 buffer을 저장한다.

// Array는 크기가 템플릿 인수로 있기에, 이걸 cpp로 하면 자유도가 너무 떨어지므로 ipp로.

template <typename T, int N>
class Array
{
public:
	explicit Array();

	~Array();

	void push_back(const T& data);

	T& operator[](int index); // 사용 편의 + 유효 범위 체크 가능(getter)

	int size() const;

	void PrintAllElements();

private:
	T _buffer[N];
	int _size = 0;
};

#include "Array.ipp"