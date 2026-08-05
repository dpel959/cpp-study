#include "pch.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "GameScene.h"

void SceneManager::Init()
{
}

void SceneManager::Update()
{
	if (_scene)
	{
		_scene->Update();
	}
}

void SceneManager::Render(HDC hdc)
{
	if (_scene)
	{
		_scene->Render(hdc);
	}
}

void SceneManager::Clear()
{
	SAFE_DELETE(_scene);
}

void SceneManager::ChangeScene(SceneType sceneType)
{
	if (_sceneType == sceneType)
	{
		return;
	}

	Scene* newScene = nullptr;

	switch (sceneType)
	{
	case SceneType::DevScene:
		newScene = new DevScene();
		break;
	case SceneType::GameScene:
		newScene = new GameScene();
		break;
	}

	// use-after-free 를 막기 위해.
	// 이 3줄 패턴이 많이 쓰인다. 매크로로 지정해주면 좋다.
	SAFE_DELETE(_scene);

	_scene = newScene;
	_sceneType = sceneType;

	newScene->Init();
}