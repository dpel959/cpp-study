#pragma once
#include "Scene.h"
class MenuScene : public Scene
{
public:
	MenuScene();
	virtual ~MenuScene() override;

	// 강의와 차별점 : 웬만하면 표준인 = 0; 을 사용하자. abstract는 MSVC 자체의 확장 문법이다.
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
};