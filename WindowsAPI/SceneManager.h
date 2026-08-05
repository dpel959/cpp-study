#pragma once

class SceneManager
{
	DECLARE_SINGLE(SceneManager)
	
public:
	void Init();
	void Update();
	void Render(HDC hdc);

	void ChangeScene(SceneType sceneType);
	void Clear();

private:

	// 근데 왜 씬이라는 것을 하나 더 들고 있는 것일까? 그냥 오브젝트들을 가지고 있으면 되는데.
	// 그렇다. 바로 그 '씬'마다, 가져야 할 오브젝트들, 그리고 해야하는 동작, 지켜야하는 규칙, 보여줘야 하는 화면 등이 달라서 그렇다.
	// 그냥 슥 바꿔치면 모든 것을 아주 편하게 바꿔줄 수 있는 것이다.
	class Scene* _scene; // 현재 실행하는 씬!
	SceneType _sceneType = SceneType::None;
};