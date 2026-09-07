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
	GET_SINGLE(ObjectManager).CreateObject<Monster>();
}

void GameScene::Update()
{
	GET_SINGLE(ObjectManager).Update();
}

void GameScene::Render(HDC hdc)
{
	GET_SINGLE(ObjectManager).Render(hdc);
}
