#pragma once
#include "Scene.h"

class EditScene : public Scene
{
public:
	EditScene();
	virtual ~EditScene() override;
	
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
private:
	//POINT는 int32 x, y 담는 거다.
	std::vector<std::pair<POINT, POINT>> _lines;

	bool _setOrigin = true;
	POINT _lastPos = {};
};