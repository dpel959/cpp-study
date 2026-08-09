#pragma once
#include "Object.h"

//원래 상속은 Creature / Projectile로 한번 더 나누는 게 좋지만 지금은 편의상 다 Object로.
class Player : public Object
{
public:
	Player();
	virtual ~Player() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
};