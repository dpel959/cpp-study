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
	const float deltaTime = GET_SINGLE(TimeManager).GetDeltaTime();

	// 강의와 차별점 : 일정 가속도 운동 공식을 사용해 프레임률에 따른 궤적 차이를 줄인다.
	// 화면 좌표계에서는 오른쪽과 아래쪽이 각각 +X, +Y 방향이다.
	_pos += _speed * deltaTime + _acceleration * (0.5f * deltaTime * deltaTime);
	_speed += _acceleration * deltaTime;

	FortressScene* scene = dynamic_cast<FortressScene*>(GET_SINGLE(SceneManager).GetCurrentScene());
	if (scene != nullptr && scene->ResolveBullet(this))
	{
		return;
	}
}

void Bullet::Render(HDC hdc)
{
	Utils::DrawCircle(hdc, _pos, static_cast<int32>(_radius));
}
