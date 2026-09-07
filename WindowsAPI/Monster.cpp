#include "pch.h"
#include "Monster.h"
#include "InputManager.h"
#include <algorithm>

Monster::Monster() : Object(ObjectType::Monster)
{

}

Monster::~Monster()
{
	if (_lookPen != nullptr)
	{
		::DeleteObject(_lookPen);
		_lookPen = nullptr;
	}
}

void Monster::Init()
{
	_stat = Stat{ 100,100,10 };

	_pos = Pos{ 400,300 };

	_lookPos = Pos{ 400,70 };

	_lookDir = _lookPos - _pos;
	_lookDir.Normalize();

	_lookPen = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
}

void Monster::Update()
{

}

void Monster::Render(HDC hdc)
{
	Utils::DrawCircle(hdc, _pos, 100);

	// frontDir
	HPEN oldpen = static_cast<HPEN>(::SelectObject(hdc, _lookPen));

	{
		Utils::DrawLine(hdc, _pos, _lookPos);
	}

	::SelectObject(hdc, oldpen);

	// 역삼각함수를 이용해, mousePos가 Monster의 뒤에 있는지, 안 치고 있는지를 판별하자!
	// (몬헌의 뒷치기 보너스 비슷한.)
	Vector mousePos = GET_SINGLE(InputManager).GetMousePos();
	Vector monsterToMouseDir = mousePos - _pos;
	monsterToMouseDir.Normalize(); // 이 벡터로 해야하는 게 뭘까? 바로 몬스터의 '정면'(lookDir)과 이 벡터의 각도 구함이다.
	
	// 근데 각을 어떻게 구해요? 애초에 cos이나 sin 값을 알아야 역삼각함수를 응용하는 거 아니에요?
	// 내적을 쓴다. 지금 각 벡터를 정규화해서 크기가 1이다. 그러므로 |v1||v2|cosθ 에서 cosθ만 남는다.

	float dot = _lookDir.Dot(monsterToMouseDir); // 이게 cosθ.
	dot = std::clamp(dot, -1.0f, 1.0f); // 평행할때는 내적값이 1.00001f가 되어 acos가 Nan이 되어버릴수가 있다. clamp 처리.
	float radian = ::acos(dot); // acos == arccos 그리고, 결과값은 당연히 'radian'체계로 뱉는다.
	// π는 180도랑 똑같은데, 이게 뭔 소리인가 싶을텐데, π는 숫자로는 3.14... 이다. 이걸로 돌려준다고!
	// π rad = 3.14... rad = 180도

	float angle = radian * 180 / PI;

	// 어, 그런데 180도를 넘어가면 180도에서 줄어드는데요? -> acos의 범위는 0 ~ π 이므로! 여기서 못 벗어난다.
	// 그래서 180도 이상 돌면, 반대쪽에서 돈 것과 마찬가지인 각도가 된다. 그런데 190도 이상을 원한다면 어떻게 할까?
	
	// 1) 특정 축에 의존하는 방식. 비교 벡터가 y축 위를 향할 때만 x < 0을 이용할 수 있어서 범용적이지 않다.

	//if (monsterToMouseDir.x < 0)
	//{
	//	angle = 360 - angle;
	//}

	// 그러면 알 것이다. '축'을 중심으로 보는 게 아니라, '나와 내적을 하는 벡터'를 기준으로 내가 왼쪽에 있는 지, 오른쪽에 있는 지 봐야한다.
	// 2가지 방법이 있다고 한다. 하나는 사영을 내려서(내적) 방향을 확인하는 방법, 하나는 외적을 해서 확인하는 방법.
	// 그런데 외적이 더 깔끔하다. acos은 범위가 0 ~ π 이고, 외적의 sinθ의 값이 0 ~ π 동안 변하지 않는다.
	// 즉, 외적의 값이 부호가 변한다/변하지 않는다로 acos의 범위를 벗어났냐 아니냐를 판별할 수 있는 것이다.
	// cos의 '1대1 대응'의 범위와, sin의 '값의 양 음이 변하는 기준'이 같기에 생기는 시너지이다. 오히려 같은 cosθ를 이용하는 내적이 잘 안 맞는 게 아이러니.

	// 2) 외적. 

	float cross = _lookDir.Cross(monsterToMouseDir); // 보통 기준 벡터 x 타겟 벡터 순으로 외적을 해준다고 한다.
	
	// 외적의 부호가 바뀌었다면, 그제서야 180도를 넘어갔다는 것이다. 부호가 양 인지 음 인지는 엔진과 외적 순서에 따라 다르다.

	if (cross > 0)
	{
		angle = 360 - angle;
	}

	{
		std::wstring str = std::format(L"[angle  : {0}]", angle);
		Utils::DrawTextW(hdc, { 20,50 }, str);
	}

}
