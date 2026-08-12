#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ObjectManager.h"
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

	if (_playerTurn == false)
	{
		return;
	}

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

	if (GET_SINGLE(InputManager).GetButton(KeyType::W))
	{
		//_barrelangle = aTime * _stat.speed;
	}


	if (GET_SINGLE(InputManager).GetButton(KeyType::S))
	{
		//_pos.y += deltaTime * _stat.speed;
	}

	if (GET_SINGLE(InputManager).GetButton(KeyType::Q))
	{

	}

	if (GET_SINGLE(InputManager).GetButton(KeyType::E))
	{

	}

	if (GET_SINGLE(InputManager).GetButtonDown(KeyType::SpaceBar))
	{

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
