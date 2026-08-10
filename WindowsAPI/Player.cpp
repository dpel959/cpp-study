#include "pch.h"
#include "Player.h"
#include "Utils.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "Missile.h"
#include "ResourceManager.h"
#include "LineMesh.h"

Player::Player() : Object(ObjectType::Player)
{

}

Player::~Player()
{
}

void Player::Init()
{
	_stat.hp = 100;
	_stat.maxHp = 100;
	_stat.speed = 500;

	_pos.x = 400;
	_pos.y = 500;

}

void Player::Update()
{
	float deltaTime = GET_SINGLE(TimeManager).GetDeltaTime();

	// 거리 = 시간 * 속도 !

	if (GET_SINGLE(InputManager).GetButton(KeyType::A))
	{
		_pos.x -= deltaTime * _stat.speed;
	}


	if (GET_SINGLE(InputManager).GetButton(KeyType::D))
	{
		_pos.x += deltaTime * _stat.speed;
	}

	if (GET_SINGLE(InputManager).GetButton(KeyType::W))
	{
		_pos.y -= deltaTime * _stat.speed;
	}


	if (GET_SINGLE(InputManager).GetButton(KeyType::S))
	{
		_pos.y += deltaTime * _stat.speed;
	}

	if (GET_SINGLE(InputManager).GetButton(KeyType::Q))
	{
		_barrelAngle += 10 * deltaTime;
	}

	if (GET_SINGLE(InputManager).GetButton(KeyType::E))
	{
		_barrelAngle += -10 * deltaTime;
	}

	if (GET_SINGLE(InputManager).GetButtonDown(KeyType::SpaceBar))
	{
		Missile* missile = GET_SINGLE(ObjectManager).CreateObject<Missile>();
		missile->SetPos(_pos);
		missile->SetAngle(_barrelAngle);
		GET_SINGLE(ObjectManager).Add(missile);
	}
}

void Player::Render(HDC hdc)
{
	const LineMesh* mesh = GET_SINGLE(ResourceManager).GetLineMesh(L"Player");
	if (mesh != nullptr)
	{
		mesh->Render(hdc, _pos);
	}

	// 계속 delete 시켜서 성능을 먹기 보다는 유지시키는 게 좋다.
	// static은 프로그램이 종료되면 알아서 회수를 해주기에, DeleteObject를 할 필요는 없지만, 습관상 해주면 좋다.
	// 하지만 일단은 패스.

	static HPEN pen = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN oldPen = static_cast<HPEN>(::SelectObject(hdc, pen)); // SelectObject가 바꿔치기 용도네.

	Utils::DrawLine(hdc, _pos, GetFirePos()); 

	::SelectObject(hdc, oldPen);
}

// 각도를 알때, x는 cos, y는 sin.
Pos Player::GetFirePos()
{
	Pos firePos = _pos;

	// 놀랍게도 cos는 기본 지원.
	// cos을 왜 곱하냐? 'cos 는 그 각도의 빗변 1에 대한 가로 값의 비율이니까.' 
	// 그래서 실제로 적용될 길이 * 비율을 곱해줘야 값이 나오는 것이다.

	// 어? 근데 왜 y는 -를 취해요? : WindowsAPI에서 y값이 양수면 아래로 향해서.
	firePos += {_barrelLength * ::cos(_barrelAngle), -_barrelLength * ::sin(_barrelAngle)};

	return firePos;
}