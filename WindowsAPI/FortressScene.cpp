#include "pch.h"
#include "FortressScene.h"
#include "UIManager.h"
#include "Player.h"
#include "LineMesh.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "ObjectManager.h"
#include "TimeManager.h"

FortressScene::FortressScene()
{
}

FortressScene::~FortressScene()
{
}

void FortressScene::Init()
{
	GET_SINGLE(UIManager).Init();

	{
		Player* player = GET_SINGLE(ObjectManager).CreateObject<Player>();
		player->SetPlayerType(PlayerType::MissileTank);

		GET_SINGLE(ObjectManager).Add(player);

		player->SetPos(Vector{ 100, 400 });
		player->SetPlayerId(0);
		player->SetPlayerTurn(true);
	}

	{
		Player* player = GET_SINGLE(ObjectManager).CreateObject<Player>();
		player->SetPlayerType(PlayerType::CanonTank);

		GET_SINGLE(ObjectManager).Add(player);

		player->SetPos(Vector{ 700, 400 });
		player->SetPos(Vector{ 100, 400 });
		player->SetPlayerId(1);
		player->SetPlayerTurn(false);
	}

	ChangePlayerTurn(); // 초기화용
}

void FortressScene::Update()
{
	float deltaTime = GET_SINGLE(TimeManager).GetDeltaTime();

	// 개선점 : 삭제될 수 있으므로, 이것도 일단 & 안 붙임.
	std::vector<Object*> objects = GET_SINGLE(ObjectManager).GetObjects();

	for (auto* object : objects)
	{
		object->Update();
	}

	_sumTime += deltaTime;
	if (_sumTime >= 1.f)
	{
		_sumTime = 0.f;

		int32 time = GET_SINGLE(UIManager).GetRemainTime();
		time = std::max(0, time - 1);

		GET_SINGLE(UIManager).SetRemainTime(time);

		if (time == 0)
		{
			ChangePlayerTurn();
		}
	}
}

void FortressScene::Render(HDC hdc)
{
	GET_SINGLE(UIManager).Render(hdc);

	const std::vector<Object*> objects = GET_SINGLE(ObjectManager).GetObjects();
	for (Object* object : objects)
	{
		object->Render(hdc);
	}
}

// 개선점 : 이것도 플레이어 벡터를 저장해주던가, 혹은 플레이어 수를 저장해주어야할 것이다.
void FortressScene::ChangePlayerTurn()
{
	// round-robin 방식
	_playerTurn = (_playerTurn + 1) % 2;
	
	const std::vector<Object*> objects = GET_SINGLE(ObjectManager).GetObjects();

	// 개선점 : Object를 또 굳이 다 돌아야한다... 이것도 좋지 않은 점
	// Player임을 확인하고, PlayerId도 확인하고 2중체크를 해야 함.
	for (Object* object : objects)
	{
		if (object->GetObjectType() != ObjectType::Player)
		{
			continue;
		}

		// static_cast로 하는 이유는, 이미 위에서 Player을 걸러줘서. dynamic_cast는 비용이 너무 아깝다.
		Player* player = static_cast<Player*>(object);
		if (player->GetPlayerId() == _playerTurn)
		{
			player->SetPlayerTurn(true);
		}
		else
		{
			player->SetPlayerTurn(false);
		}
	}

	// 개선점 : 이것도 데이터로 빼야 함..
	GET_SINGLE(UIManager).SetRemainTime(10);
	GET_SINGLE(UIManager).SetStaminaPercent(100.f);
	GET_SINGLE(UIManager).SetPowerPercent(0.f);
	GET_SINGLE(UIManager).SetWindPercent(RandomUtils::GetRandomFloat(-100, 100));
}
