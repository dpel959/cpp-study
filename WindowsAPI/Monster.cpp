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
	Vector mousePos = GET_SINGLE(InputManager).GetMousePos();

	// start -> end
	Vector v1 = _end - _start; // start에서 end까지의 벡터

	// start -> mousePos
	Vector v2 = mousePos - _start; // start에서 mousePos까지의 벡터

	// 우리는 v2 cosθ를 구할 것이다.
	float maxLength = v1.Length();
	v1.Normalize();
	float dot = v1.Dot(v2);
	Pos pos = _start + v1 * dot; // 시작점에서 사영만큼 이동!

	// dot < 0 이면 270도 미만의 둔각.  
	// v1이 normalize 되어있기에, dot > maxLength는 예각이라도 사영이 v1을 넘어가면 컷함
	// 즉, 선상에서만 돌아다니게 함.

	// 예시로, VR 게임에서 우리가 손을 딱 x축에 맞춰서 서랍을 당길 리가 없다.
	// 그런 것에서, 서랍의 움직임 같은 것에 사용 가능.

	if (dot < 0 || dot > maxLength) 
	{
		return;
	}

	_pos = pos;

	// 이렇게 하면, 그 선 상에서 mousePos를 따라 이동한다!
}

void Monster::Render(HDC hdc)
{
	Utils::DrawCircle(hdc, _pos, 50);

	static HPEN pen = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN oldpen = static_cast<HPEN>(::SelectObject(hdc, pen));

	Utils::DrawLine(hdc, _start, _end);

	::SelectObject(hdc, oldpen);
}