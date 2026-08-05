#include "pch.h"
#include "Utils.h"

void Utils::DrawText(HDC hdc, Pos pos, const std::wstring str)
{
	::TextOut(hdc, static_cast<int32>(pos.x), static_cast<int32>(pos.y), str.c_str(), str.size());
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