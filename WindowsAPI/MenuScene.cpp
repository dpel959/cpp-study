#include "pch.h"
#include "MenuScene.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "LineMesh.h"

MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::Init()
{
}

void MenuScene::Update()
{
	if (GET_SINGLE(InputManager).GetButtonDown(KeyType::E))
	{
		GET_SINGLE(SceneManager).ChangeScene(SceneType::FortressScene);
	}
}

void MenuScene::Render(HDC hdc)
{
	const LineMesh* mesh = GET_SINGLE(ResourceManager).GetLineMesh(L"Menu");

	if (mesh != nullptr)
	{
		mesh->Render(hdc, Pos{ 0, 0 });
	}
}