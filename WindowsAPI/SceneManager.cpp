#include "pch.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "EditScene.h"
#include "MenuScene.h"
#include "FortressScene.h"
#include "ObjectManager.h"

void SceneManager::Init()
{
}

void SceneManager::Update()
{
	_isUpdating = true;

	if (_scene != nullptr)
	{
		_scene->Update();
	}

	_isUpdating = false;
	ApplyPendingSceneChange();
}

void SceneManager::Render(HDC hdc)
{
	if (_scene != nullptr)
	{
		_scene->Render(hdc);
	}
}

void SceneManager::ChangeScene(SceneType sceneType)
{
	if (sceneType == SceneType::None)
	{
		return;
	}

	if (_pendingSceneType == sceneType ||
		(_pendingSceneType == SceneType::None && _sceneType == sceneType))
	{
		return;
	}

	_pendingSceneType = sceneType;

	if (_isUpdating == false)
	{
		ApplyPendingSceneChange();
	}
}

void SceneManager::RestartCurrentScene()
{
	if (_sceneType == SceneType::None)
	{
		return;
	}

	_pendingSceneType = _sceneType;
	if (_isUpdating == false)
	{
		ApplyPendingSceneChange();
	}
}

void SceneManager::Clear()
{
	GET_SINGLE(ObjectManager).Clear();
	_scene.reset();
	_sceneType = SceneType::None;
	_pendingSceneType = SceneType::None;
	_isUpdating = false;
}

void SceneManager::ApplyPendingSceneChange()
{
	if (_pendingSceneType == SceneType::None)
	{
		return;
	}

	const SceneType nextSceneType = _pendingSceneType;
	_pendingSceneType = SceneType::None;

	std::unique_ptr<Scene> nextScene = CreateScene(nextSceneType);
	if (nextScene == nullptr)
	{
		return;
	}

	GET_SINGLE(ObjectManager).Clear();
	_scene = std::move(nextScene);
	_sceneType = nextSceneType;
	_scene->Init();
}

std::unique_ptr<Scene> SceneManager::CreateScene(SceneType sceneType) const
{
	switch (sceneType)
	{
	case SceneType::DevScene:
		return std::make_unique<DevScene>();
	case SceneType::EditScene:
		return std::make_unique<EditScene>();
	case SceneType::MenuScene:
		return std::make_unique<MenuScene>();
	case SceneType::FortressScene:
		return std::make_unique<FortressScene>();
	default:
		return nullptr;
	}
}
