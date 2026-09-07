#pragma once
class Utils
{
public:
	static void DrawText(HDC hdc, Pos pos, const std::wstring& str);

	static void DrawRect(HDC hdc, Pos pos, int32 w, int32 h);

	static void DrawCircle(HDC hdc, Pos pos, int32 radius);

	static void DrawLine(HDC hdc, Pos from, Pos to);
};

namespace RandomUtils
{
	int32 GetRandomInt(int start, int end);
	float GetRandomFloat(float start, float end);
}
