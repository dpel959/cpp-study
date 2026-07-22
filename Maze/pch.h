#pragma once
#include "Types.h"
#include <Windows.h>
#include <iostream>
using std::cout;

struct Pos
{
	Pos() {}
	Pos(int32 y, int32 x) : y(y), x(x) {}

	bool operator==(const Pos& other)
	{
		return y == other.y && x == other.x;
	}

	bool operator!=(const Pos& other)
	{
		return !(*this == other);
	}

	Pos operator+(const Pos& other)
	{
		Pos ret;
		ret.y = y + other.y;
		ret.x = x + other.x;
		return ret;
	}

	Pos& operator+=(const Pos& other)
	{
		y += other.y;
		x += other.x;
		return *this;
	}

	int32 y = 0;
	int32 x = 0;
};

/// <summary>
// enum은 연관된 상수를 만드는 것으로 이용할 수 있지만, 단점이 남는다.
// 그것을 막기 위해 namespace 안에 enum을 넣는다. Dir::UP 이러한 방식으로 사용 가능.
/// </summary>

namespace Dir
{
	enum Type : uint8
	{
		UP,
		LEFT,
		DOWN,
		RIGHT,

		MAX_COUNT
	};
}