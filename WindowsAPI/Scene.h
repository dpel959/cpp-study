#pragma once

// 씬에 뭔가 그려줘야 할때, 물체의 처리 등
class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};

	// 강의와 차별점 : 웬만하면 표준인 = 0; 을 사용하자. abstract는 MSVC 자체의 확장 문법이다.
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render(HDC hdc) = 0;

protected:

};