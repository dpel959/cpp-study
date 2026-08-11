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
	Monster* monster = GET_SINGLE(ObjectManager).CreateObject<Monster>();
	GET_SINGLE(ObjectManager).Add(monster);
}

void GameScene::Update()
{
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
	const std::vector<Object*>& objects = GET_SINGLE(ObjectManager).GetObjects();
	
	for (Object* object : objects)
	{
		object->Render(hdc); // 사실 오브젝트 매니저의 Udpate에서 돌려줘도 된다.
	}
}