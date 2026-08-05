#include "pch.h"
#include "TimeManager.h"

void TimeManager::Init()
{
	//시간 측정하는 건 굉장히 많은데, GetTickCount64는 어느 시점부터 지금까지의 ms를 재준다.
	//B시점 - A시점으로 지나간 시간을 구할 수 있...긴 한데. 정밀도가 좀 떨어진다.
	//QueryPerformance를 사용한다.
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(& _frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount)); // CPU 클럭
}


void TimeManager::Update()
{
	//::QueryPerformanceCounter을 주로 써줄 것.

	uint64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount)); // currentCount에 값을 받아온다.

	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency); // CPU 클럭 / frequency 하면 시간이 나온다고 문서에 나와있다. 그대로 쓰면 됨.
	// 참고로, 이 단위는 s 임. ms로 하고 싶으면 1000을 곱하거나 하자 (근데 왜 이건 s인데 ms 구해주는 GetTickCount보다 정밀도가 낮다는 거지)

	_prevCount = currentCount; // _prevCount도 갱신해야, 다음 currentCount와 - 할때도 올바르게 구해질 것

	++_frameCount; // 내가 (메인 루프가) 얼마나 갱신되었는가?
	_frameTime += _deltaTime; // 그동안 얼마나 지났는가?

	// 1초를 경과했다면! (f'ps' 이므로)
	if (_frameTime >= 1.f)
	{
		_fps = static_cast<uint32>(_frameCount / _frameTime);

		_frameCount = 0;
		_frameTime = 0.f;
	}
}