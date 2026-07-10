#pragma once
#include <vector>
#define OUT

// *************************
// 파라미터로 받아오는 값이 있다면 OUT을 써주면 가독성이 올라간다.
// *************************

namespace Random
{
	int GetRandomInt(int min, int max);
	void GetSample(std::vector<int>& vec, int chooseNum, OUT std::vector<int>& dest);
};