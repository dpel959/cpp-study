#include "pch.h"
#include "Monster.h"
#include "Utils.h"
#include "InputManager.h"

Monster::Monster() : Object(ObjectType::Monster)
{

}

Monster::~Monster()
{
}

void Monster::Init()
{
	_stat = { 100,100,10 };
}

void Monster::Update()
{

}

void Monster::Render(HDC hdc)
{
	Vector mousePos = GET_SINGLE(InputManager).GetMousePos();

	static HPEN pen = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN oldpen = static_cast<HPEN>(::SelectObject(hdc, pen));

	Utils::DrawLine(hdc, _pt1, _pt2);
	Utils::DrawLine(hdc, _pt2, _pt3);
	Utils::DrawLine(hdc, _pt3, _pt1);

	::SelectObject(hdc, oldpen);

	Utils::DrawLine(hdc, _pt1, mousePos);
	Utils::DrawLine(hdc, _pt2, mousePos);
	Utils::DrawLine(hdc, _pt3, mousePos);

	Vector v12 = _pt2 - _pt1;
	Vector v1m = mousePos - _pt1;
	Vector v13 = _pt3 - _pt1;

	// 사실 정규화 해줄 필요는 없지만, 너무 계산값을 크게 하지 않기 위해.
	v12.Normalize();
	v1m.Normalize();
	v13.Normalize();

	// 원래 결과 값으로 벡터가 나왔겠지만, 지금은 계수만 이용한다.
	// 12 와 1m의 외적, 1m과 13의 외적.
	float c1 = v12.Cross(v1m);
	float c2 = v1m.Cross(v13);

	// 부채꼴 안에 있을 때! 만약 둘다 음수라면 부채꼴 밖에 있을 것이다.
	if (c1 >= 0 && c2 >= 0)
	{
		// 부채꼴 안
	}
	else
	{
		// 점이 부채꼴 외부에 있을 때
	}

	std::wstring str = std::format(L"c1({0}), c2({1})", c1, c2);
	Utils::DrawTextW(hdc, { 20,50 }, str);
}