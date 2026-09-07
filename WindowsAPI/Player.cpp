#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "UIManager.h"
#include "LineMesh.h"
#include "Bullet.h"
#include "Terrain.h"
#include <algorithm>

Player::Player() : Object(ObjectType::Player)
{

}

Player::~Player()
{
	if (_turnBrush != nullptr)
	{
		::DeleteObject(_turnBrush);
		_turnBrush = nullptr;
	}
}

void Player::Init()
{
	_stat.hp = 100;
	_stat.maxHp = 100;
	_stat.speed = 500;
	_radius = 38.f;
	_turnBrush = ::CreateSolidBrush(RGB(250, 236, 197));

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

	_pos.x = std::clamp(_pos.x, _radius, GWinSizeX - _radius);
	SnapToGround();

	if (GET_SINGLE(InputManager).GetButton(KeyType::W))
	{
		// min, max보다 clamp 처리가 깔끔!

		_fireAngle = std::clamp(
			_fireAngle + GPlayerAngleSpeed * deltaTime,
			0.f,
			GPlayerMaxFireAngle);
	}

	if (GET_SINGLE(InputManager).GetButton(KeyType::S))
	{
		_fireAngle = std::clamp(
			_fireAngle - GPlayerAngleSpeed * deltaTime,
			0.f,
			GPlayerMaxFireAngle);
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
		powerPercent = std::min(100.f, powerPercent + GPowerChargeSpeed * deltaTime);
		GET_SINGLE(UIManager).SetPowerPercent(powerPercent);
	}

	if (GET_SINGLE(InputManager).GetButtonUp(KeyType::SpaceBar))
	{
		_playerTurn = false;

		float powerPercent = GET_SINGLE(UIManager).GetPowerPercent();
		float speed = GBulletPowerToSpeed * powerPercent;
		float angle = GET_SINGLE(UIManager).GetBarrelAngle();
		float windPercent = GET_SINGLE(UIManager).GetWindPercent();
		Vector shotDirection{
			::cos(angle * PI / 180),
			-::sin(angle * PI / 180)
		};

		Bullet* bullet = GET_SINGLE(ObjectManager).CreateObject<Bullet>();
		bullet->SetOwnerId(_playerId);
		bullet->SetPos(_pos + shotDirection * GBulletSpawnDistance);
		// 지금 angle이 호도법이 아니라 도수법으로 되어있어서, 호도법으로 바꿔줌
		bullet->SetSpeed(shotDirection * speed);
		// 발사 시점의 바람을 저장하므로, 비행 도중 UI 값이 바뀌어도 포탄의 가속도는 유지된다.
		bullet->SetAcceleration(Vector{
			GMaxWindAcceleration * windPercent / 100.f,
			GBulletGravity
		});
	}
}

void Player::Render(HDC hdc)
{
	const LineMesh* mesh = GET_SINGLE(ResourceManager).GetLineMesh(GetMeshKey());

	if (mesh != nullptr)
	{
		if (_dir == Dir::Left)
		{
			mesh->Render(hdc, _pos, -0.5f, 0.5f);
		}
		else
		{
			mesh->Render(hdc, _pos, 0.5f, 0.5f);
		}
	}

	if (_playerTurn)
	{
		// 현재 턴인 탱크의 위쪽에 표시한다.
		RECT rect;
		rect.bottom = static_cast<LONG>(_pos.y - 60);
		rect.left = static_cast<LONG>(_pos.x - 10);
		rect.right = static_cast<LONG>(_pos.x + 10);
		rect.top = static_cast<LONG>(_pos.y - 80);

		HBRUSH oldBrush = static_cast<HBRUSH>(::SelectObject(hdc, _turnBrush));

		::Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);

		::SelectObject(hdc, oldBrush);
	}

	const float hpRatio = static_cast<float>(_stat.hp) / static_cast<float>(_stat.maxHp);
	const RECT hpBackground = {
		static_cast<LONG>(_pos.x - 40),
		static_cast<LONG>(_pos.y - 55),
		static_cast<LONG>(_pos.x + 40),
		static_cast<LONG>(_pos.y - 47)
	};
	RECT hpFill = hpBackground;
	hpFill.right = hpFill.left + static_cast<LONG>((hpBackground.right - hpBackground.left) * hpRatio);

	HBRUSH stockBrush = static_cast<HBRUSH>(::GetStockObject(DC_BRUSH));
	HPEN stockPen = static_cast<HPEN>(::GetStockObject(DC_PEN));
	HBRUSH oldBrush = static_cast<HBRUSH>(::SelectObject(hdc, stockBrush));
	HPEN oldPen = static_cast<HPEN>(::SelectObject(hdc, stockPen));
	const COLORREF oldBrushColor = ::SetDCBrushColor(hdc, RGB(120, 45, 45));
	const COLORREF oldPenColor = ::SetDCPenColor(hdc, RGB(30, 30, 30));
	::Rectangle(hdc, hpBackground.left, hpBackground.top, hpBackground.right, hpBackground.bottom);

	if (hpFill.right > hpFill.left)
	{
		::SetDCBrushColor(hdc, RGB(75, 200, 105));
		::Rectangle(hdc, hpFill.left, hpFill.top, hpFill.right, hpFill.bottom);
	}

	::SetDCBrushColor(hdc, oldBrushColor);
	::SetDCPenColor(hdc, oldPenColor);
	::SelectObject(hdc, oldPen);
	::SelectObject(hdc, oldBrush);
}

// 실제로 직업군마다 다르게 동작하는 게 있으면, 이런 식으로 해도 된다.
std::wstring Player::GetMeshKey() const
{
	if (_playerType == PlayerType::MissileTank)
	{
		return L"MissileTank";
	}
	
	// 현재 지원하는 탱크는 MissileTank와 CanonTank 두 종류이다.
	return L"CanonTank";
}

void Player::UpdateFireAngle()
{
	// fireAngle을 굳이 플레이어마다 왜 들고 있어요? -> 플레이어마다 다르기도 하고, 플레이어의 방향따라서 반전되어서.
	// 탱크가 바라보는 방향에 맞춰 UI에 표시할 차체와 포신 각도를 변환한다.
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

void Player::TakeDamage(int32 damage)
{
	_stat.hp = std::clamp(_stat.hp - std::max(0, damage), 0, _stat.maxHp);
}

void Player::SnapToGround()
{
	if (_terrain != nullptr)
	{
		_pos.y = _terrain->GetGroundY(_pos.x) - GPlayerGroundOffset;
	}
}
