#pragma once
#include "Scene.h"
class FortressScene : public Scene
{
public:
	FortressScene();
	virtual ~FortressScene() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	// 개선점 : 이것도 지정해서 바꿔주면 좋겠지.
	void ChangePlayerTurn();

private:
	int32 _playerTurn = 1;
	float _sumTime = 0.f;
};