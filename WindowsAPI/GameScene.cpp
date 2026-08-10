#include "pch.h"
#include "GameScene.h"
#include "Player.h"
#include "Monster.h"
#include "ObjectManager.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Init()
{
	//_player = new Player();
	//_player->Init();

	// player을 하드코딩해서 들고 있지 말고, 다른 Object와 같이 관리하기 위해 Object로 player을 들고 있도록 하자.

	{
		Player* player = GET_SINGLE(ObjectManager).CreateObject<Player>();

		player->SetPos(Pos{ 400, 400 });

		GET_SINGLE(ObjectManager).Add(player);
	}

	for(int32 i = 0;i < 5; ++i)
	{
		Monster* monster = GET_SINGLE(ObjectManager).CreateObject<Monster>();

		monster->SetPos(Pos{ static_cast<float>(i + 1) * 100, 100 });

		GET_SINGLE(ObjectManager).Add(monster);
	}
}

void GameScene::Update()
{
	//if (_player != nullptr)
	//{
	//	_player->Update();
	//}

	std::vector<Object*> objects =  GET_SINGLE(ObjectManager).GetObjects();

	for (Object* object : objects)
	{
		object->Update(); // 사실 오브젝트 매니저의 Udpate에서 돌려줘도 된다.
	}



	for (Object* object : objects)
	{
		objects.push_back(object);
	}
}

void GameScene::Render(HDC hdc)
{
	//if (_player != nullptr)
	//{
	//	_player->Render(hdc);
	//}

	const std::vector<Object*>& objects = GET_SINGLE(ObjectManager).GetObjects();
	
	for (Object* object : objects)
	{
		object->Render(hdc); // 사실 오브젝트 매니저의 Udpate에서 돌려줘도 된다.
	}
}