#include "pch.h"
#include "Missile.h"
#include "Utils.h"
#include "TimeManager.h"
#include "ObjectManager.h"

Missile::Missile() : Object(ObjectType::Projectile)
{
}

Missile::~Missile()
{
}

void Missile::Init()
{
	_stat.hp = 1;
	_stat.maxHp = 1;
	_stat.speed = 600;
}

void Missile::Update()
{
	float deltaTime = GET_SINGLE(TimeManager).GetDeltaTime();

	_pos.x += deltaTime * ::cos(_angle) * _stat.speed;
	_pos.y -= deltaTime * ::sin(_angle)* _stat.speed;
	
	// 충돌. 유니티나 언리얼같이 어디 반경에 들어오면 감지하고 그런거 없다. 씬 전부를 뒤져봐야한다...
	
	// 이거 ㅄ 같이 &로 받아줘야한다. 추가의 영향을 받지 않기 위해...
	const std::vector<Object*> objects = GET_SINGLE(ObjectManager).GetObjects();
	for (Object* object : objects)
	{
		if (object == this)
		{
			continue;
		}

		if (object->GetObjectType() != ObjectType::Monster)
		{
			continue;
		}

		Pos p1 = GetPos();
		Pos p2 = object->GetPos();

		const float dx = p1.x - p2.x;
		const float dy = p1.y - p2.y;
		float dist = sqrt(dx * dx + dy * dy);

		if (dist < 25)
		{
			GET_SINGLE(ObjectManager).Remove(object);
			GET_SINGLE(ObjectManager).Remove(this);
			return;
		}
	}


	// 화면 끝을 넘어가면 삭제. 이것도 각도를 자유롭게 할 수 있으면 바꿔줘야한다.
	if (_pos.y < -200)
	{
		GET_SINGLE(ObjectManager).Remove(this);

		// 근데, 내가 delete 됐으면 여기 뒤 코드는 어떻게 될까? 
		// 놀랍게도 돌아가기는 한다. 바로 재활용을 위해 OS에 반환을 안해주기 때문이다.
		// 그런데, 그게 당연히 좋은 건 아니다. 무효화된 곳을 건드리고 있는 것이기 때문이다.

		// 그러니까, 1. 자신을 지웠으면 바로 return을 쳐라!
		// 2. 아니면 지워질때 true를 반환해, 외부에서 감지시켜 외부에서 지워라.
		// 3. 그냥 스마트 포인터 좀 써라!

		return;
	}
}

void Missile::Render(HDC hdc)
{
	Utils::DrawCircle(hdc, _pos, 25);
}
