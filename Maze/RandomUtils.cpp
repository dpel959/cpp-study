#include "pch.h"
#include "RandomUtils.h"
#include <random>

// 강의와 차별점 : 공통적으로 쓸 random 요소들, 그냥 공통적으로 namespace로 함

// 이 .h 말고 접근 금지. static이라 보면 됨
namespace
{
	std::random_device rd;
	std::mt19937 gen(rd());
}

namespace Random
{
	int GetRandomInt(int start, int end)
	{
		std::uniform_int_distribution<int> dis(start, end);
		return dis(gen);
	}
}