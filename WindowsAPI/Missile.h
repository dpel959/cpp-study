#pragma once
#include "Object.h"

class Missile : public Object
{
public:
	Missile();
	virtual ~Missile() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	
	void SetAngle(float angle) { _angle = angle; }
	
protected:
	float _angle = 0.f;
	float _sumTime = 0.f;
	// ObjectManager가 소유한 대상을 잠시 가리키는 포인터이다.
	// 사용하기 전에 ObjectManager::IsAlive로 삭제 여부를 확인한다.
	Object* _target = nullptr;
};
