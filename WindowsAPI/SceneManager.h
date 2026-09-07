#pragma once
#include "Scene.h"
#include <memory>

class SceneManager
{
	DECLARE_SINGLE(SceneManager)

public:
	void Init();
	void Update();
	void Render(HDC hdc);

	// 강의와 차별점 : Update 도중에는 현재 Scene을 바로 파괴하지 않고, 프레임의 Update가 끝난 뒤 교체한다.
	void ChangeScene(SceneType sceneType);
	void RestartCurrentScene();
	void Clear();

	Scene* GetCurrentScene() const { return _scene.get(); }

private:
	void ApplyPendingSceneChange();
	std::unique_ptr<Scene> CreateScene(SceneType sceneType) const;

private:
	std::unique_ptr<Scene> _scene;
	SceneType _sceneType = SceneType::None;
	SceneType _pendingSceneType = SceneType::None;
	bool _isUpdating = false;
};
