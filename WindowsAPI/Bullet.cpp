#include "pch.h"
#include "Bullet.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "SceneManager.h"
#include "FortressScene.h"

Bullet::Bullet() : Object(ObjectType::Projectile)
{
}

Bullet::~Bullet()
{
}

void Bullet::Init()
{
	_radius = 20.f;
}

void Bullet::Update()
{
	float deltaTime = GET_SINGLE(TimeManager).GetDeltaTime();

	// 개선점 : 그냥 일직선. 바람도 중력도 없음
	_pos += _speed * deltaTime;

	// 개선점 : 화면 벗어났을때 너무 하드함. 그리고 X 적용도 안 됨.
	if (_pos.y > GWinSizeY * 1.0 || _pos.y < -GWinSizeY * 1.0)
	{
		// 다른 플레이어로 바꿔줘!
		FortressScene* scene = dynamic_cast<FortressScene*>(GET_SINGLE(SceneManager).GetCurrentScene());
		if (scene != nullptr)
		{
			scene->ChangePlayerTurn();
		}

		GET_SINGLE(ObjectManager).Remove(this);
		return;
	}
}

void Bullet::Render(HDC hdc)
{
	Utils::DrawCircle(hdc, _pos, static_cast<int32>(_radius));
}
