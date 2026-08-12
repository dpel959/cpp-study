#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "UIManager.h"
#include "LineMesh.h"
#include "Bullet.h"

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

	if (_playerTurn == false)
	{
		return;
	}

	UpdateFireAngle();
	
	if (GET_SINGLE(InputManager).GetButton(KeyType::A))
	{
		_pos.x -= deltaTime * _stat.speed;
		_dir = Dir::Left;
	}


	if (GET_SINGLE(InputManager).GetButton(KeyType::D))
	{
		_pos.x += deltaTime * _stat.speed;
		_dir = Dir::Right;
	}

	// 개선점 : 이것도 움직이는 속도가 하드코딩임. 고쳐주자
	if (GET_SINGLE(InputManager).GetButton(KeyType::W))
	{
		// min, max보다 clamp 처리가 깔끔!

		_fireAngle = std::clamp(_fireAngle + 50 * deltaTime, 0.f, 75.f);
	}

	if (GET_SINGLE(InputManager).GetButton(KeyType::S))
	{
		_fireAngle = std::clamp(_fireAngle - 50 * deltaTime, 0.f, 75.f);
	}

	if (GET_SINGLE(InputManager).GetButton(KeyType::Q))
	{

	}

	if (GET_SINGLE(InputManager).GetButton(KeyType::E))
	{

	}

	if (GET_SINGLE(InputManager).GetButton(KeyType::SpaceBar))
	{
		float powerPercent = GET_SINGLE(UIManager).GetPowerPercent();
		powerPercent = std::min(100.f, powerPercent + 100 * deltaTime);
		GET_SINGLE(UIManager).SetPowerPercent(powerPercent);
	}

	// 개선점 : 이것도 스피드가 하드.. 고치자
	if (GET_SINGLE(InputManager).GetButtonUp(KeyType::SpaceBar))
	{
		_playerTurn = false;

		float powerPercent = GET_SINGLE(UIManager).GetPowerPercent();
		float speed = 10.f * powerPercent;
		float angle = GET_SINGLE(UIManager).GetBarrelAngle();

		Bullet* bullet = GET_SINGLE(ObjectManager).CreateObject<Bullet>();
		bullet->SetPos(_pos);
		// 지금 angle이 호도법이 아니라 도수법으로 되어있어서, 호도법으로 바꿔줌
		bullet->SetSpeed(Vector{ speed * ::cos(angle * PI / 180), -speed * ::sin(angle * PI / 180)});
		GET_SINGLE(ObjectManager).Add(bullet);
	}
}

void Player::Render(HDC hdc)
{
	const LineMesh* mesh = GET_SINGLE(ResourceManager).GetLineMesh(GetMeshKey());

	if (mesh != nullptr)
	{
		if (_dir == Dir::Left)
		{
			mesh->Render(hdc, _pos, 0.5f, 0.5f);
		}
		else
		{
			mesh->Render(hdc, _pos, -0.5f, 0.5f);
		}
	}

	if (_playerTurn)
	{
		// 개선점 : 물론, 이런식으로 하드 코딩하면 안되긴 한다...
		RECT rect;
		rect.bottom = static_cast<LONG>(_pos.y - 60);
		rect.left = static_cast<LONG>(_pos.x - 10);
		rect.right = static_cast<LONG>(_pos.x + 10);
		rect.top = static_cast<LONG>(_pos.y - 80);

		static HBRUSH brush = ::CreateSolidBrush(RGB(250, 236, 197));
		HBRUSH oldBrush = static_cast<HBRUSH>(::SelectObject(hdc, brush));

		::Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);

		::SelectObject(hdc, oldBrush);
	}

	// 강의와 차별점 : 계속 delete 시켜서 성능을 먹기 보다는 유지시키는 게 좋다.
	// static은 프로그램이 종료되면 알아서 회수를 해주기에, DeleteObject를 할 필요는 없지만, 습관상 해주면 좋다.
	// 하지만 일단은 패스.

	static HPEN pen = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN oldPen = static_cast<HPEN>(::SelectObject(hdc, pen)); // SelectObject가 바꿔치기 용도네.

	::SelectObject(hdc, oldPen);
}

// 실제로 직업군마다 다르게 동작하는 게 있으면, 이런 식으로 해도 된다.
std::wstring Player::GetMeshKey()
{
	if (_playerType == PlayerType::MissileTank)
	{
		return L"MissileTank";
	}
	
	// 개선점 : 지금은 2개밖에 없으므로.
	return L"CanonTank";
}

void Player::UpdateFireAngle()
{
	// fireAngle을 굳이 플레이어마다 왜 들고 있어요? -> 플레이어마다 다르기도 하고, 플레이어의 방향따라서 반전되어서.
	// 개선점 : 어후 PlayerAngle은 왜 얘가 들고있대냐..
	if (_dir == Dir::Left)
	{
		GET_SINGLE(UIManager).SetPlayerAngle(180);
		GET_SINGLE(UIManager).SetBarrelAngle(180 - _fireAngle);
	}
	else
	{
		GET_SINGLE(UIManager).SetPlayerAngle(0);
		GET_SINGLE(UIManager).SetBarrelAngle(_fireAngle);
	}
}