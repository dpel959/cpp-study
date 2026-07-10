#include "Random.h"
#include <random>
#include <algorithm>

// *************************
// 기존 강의 코드와 다르게 더 고른 분포를 위해 srand, rand 대신
// random.h를 이용해 random_device와 mt19937을 이용한다.
// *************************

// *************************
// 익명 네임스페이스를 사용한다. 스코프 외에 노출되지 않으면서도, 이 cpp 안에서 사용하기 위함
// 익명 네임스페이스를 사용하면 자동으로 using namespace;가 적용된다.
// *************************

namespace
{
	std::random_device rd;
	std::mt19937 gen(rd());
}

// *************************
// sample이 난수 생성 엔진이 필요한데, 밖에서 또 생성해서 만드는 것은 비효율적이므로
// 파라미터를 쉽게 만드는 겸, sample을 wrapper 함수로 만든다.
// *************************

namespace Random
{
	int GetRandomInt(int min, int max)
	{
		std::uniform_int_distribution<int> dis(min, max);
		return dis(gen);
	}

	void GetSample(std::vector<int>& vec, int chooseNum, OUT std::vector<int>& dest)
	{
		std::sample(vec.begin(), vec.end(), std::back_inserter(dest), chooseNum, gen);
	}
}