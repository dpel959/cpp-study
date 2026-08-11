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
	// 개선점 : 당연히 이러면 댕글링 포인터에 너~무 취약하므로 shared_ptr가 맞다!
	// 이건 잘못된 코드이다. 유도 미사일이면 그 몬스터를 모두가 참조하고 있고, 사라지면 참조할 놈이 없어져버리니까!
	Object* _target = nullptr;
};