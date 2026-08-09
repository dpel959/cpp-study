#include "pch.h"
#include "Player.h"
#include "Utils.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "Missile.h"

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

	if (GET_SINGLE(InputManager).GetButtonDown(KeyType::SpaceBar))
	{
		Missile* missile = GET_SINGLE(ObjectManager).CreateObject<Missile>();
		missile->SetPos(_pos);
		GET_SINGLE(ObjectManager).Add(missile);
	}
}

void Player::Render(HDC hdc)
{
	Utils::DrawCircle(hdc, _pos, 50);
}