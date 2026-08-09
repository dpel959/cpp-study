#pragma once
#include "Scene.h"

class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene() override;

	// 강의와 차별점 : 웬만하면 표준인 = 0; 을 사용하자. abstract는 MSVC 자체의 확장 문법이다.
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	class Player* _player = nullptr;

	// 웬만하면 컨테이너는 벡터가 맞음. 그냥 메모리 일렬 GOAT도 맞는데,
	// 삭제하지 말고 오브젝트 풀링해주면 그만임.
};