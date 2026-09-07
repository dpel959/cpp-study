#include "pch.h"
#include "Utils.h"
#include <random>

namespace
{
	std::mt19937& GetRandomGenerator()
	{
		static std::random_device randomDevice;
		static std::mt19937 generator(randomDevice());
		return generator;
	}
}

void Utils::DrawText(HDC hdc, Pos pos, const std::wstring& str)
{
	::TextOut(
		hdc,
		static_cast<int32>(pos.x),
		static_cast<int32>(pos.y),
		str.c_str(),
		static_cast<int32>(str.size()));
}

void Utils::DrawRect(HDC hdc, Pos pos, int32 w, int32 h)
{
	// 원래 left top, right bottom 식이지만, 우리는 중심점을 받는 식으로 받아준다.
	// 나중에 좌표를 다룰 때 그럴 일이 많아서.
	::Rectangle(hdc, static_cast<int32>(pos.x - w / 2), static_cast<int32>(pos.y - h / 2)
		, static_cast<int32>(pos.x + w / 2), static_cast<int32>(pos.y + h / 2));
}

void Utils::DrawCircle(HDC hdc, Pos pos, int32 radius)
{
	::Ellipse(hdc, static_cast<int32>(pos.x - radius), static_cast<int32>(pos.y - radius)
		, static_cast<int32>(pos.x + radius), static_cast<int32>(pos.y + radius));
}

void Utils::DrawLine(HDC hdc, Pos from, Pos to)
{
	::MoveToEx(hdc, static_cast<int32>(from.x), static_cast<int32>(from.y), nullptr); //  마지막 파라미터는 뭐야?
	::LineTo(hdc, static_cast<int32>(to.x), static_cast<int32>(to.y));
}

// 아니 이렇게 다 int로 해줄 거면 그냥 Pos를 int로 하지 왜 flaot로 했냐? -> 그러면 세부 움직임이 적용이 안 될 때가 있어서.
// 강의와 차별점 : rand() 대신 std::random을 사용한다.
// 엔진은 재사용하되 분포는 호출마다 받아온 범위로 생성하여, 첫 호출의 범위가 고정되는 문제를 피한다.

int32 RandomUtils::GetRandomInt(int start, int end)
{
	std::uniform_int_distribution<int32> distribution(start, end);
	return distribution(GetRandomGenerator());
}

float RandomUtils::GetRandomFloat(float start, float end)
{
	std::uniform_real_distribution<float> distribution(start, end);
	return distribution(GetRandomGenerator());
}
