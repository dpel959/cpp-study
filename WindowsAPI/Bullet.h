#pragma once
#include "Object.h"
class Bullet : public Object
{
public:
	Bullet();
	virtual ~Bullet() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void SetSpeed(Vector speed) { _speed = speed; }
	void SetAcceleration(Vector acceleration) { _acceleration = acceleration; }
	void SetOwnerId(int32 ownerId) { _ownerId = ownerId; }
	int32 GetOwnerId() const { return _ownerId; }

public:
	Vector _speed = {};
	Vector _acceleration = {};
	int32 _ownerId = -1;
};
