#include "pch.h"
#include "Game.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"
using std::wstring;

Game::Game()
{

}

Game::~Game()
{

}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_hdc = ::GetDC(hwnd); 
	// device context는 안 긁어와도 된다.
	// 윈도우 핸들 번호만 있으면 찾아줌.

	GET_SINGLE(TimeManager).Init();
	GET_SINGLE(InputManager).Init(_hwnd);
	GET_SINGLE(SceneManager).Init();

	GET_SINGLE(SceneManager).ChangeScene(SceneType::DevScene);
}

void Game::Update() 
{
	GET_SINGLE(TimeManager).Update();
	GET_SINGLE(InputManager).Update();
	GET_SINGLE(SceneManager).Update();
}

void Game::Render()
{
	uint32 fps = GET_SINGLE(TimeManager).GetFps();
	float deltaTime = GET_SINGLE(TimeManager).GetDeltaTime();

	{
		// 이번에는 WCHAR buffer (이건 고전이다) 말고 wstring을 써보자.
		// std::format은 printf의 단점을 없앤 아주 좋은 문법인데... C++20 부터 지원
		wstring str = std::format(L"FPS({0}), DT({1} ms)", fps, static_cast<int>(deltaTime * 1000));

		//TextOut이 구닥다리 방식이라 c_str()만 받는다.
		::TextOut(_hdc, 650, 10, str.c_str(), static_cast<int>(str.size()));

		//::Rectangle(_hdc, 200, 200, 400, 400);
	}


	{
		POINT mousePos = GET_SINGLE(InputManager).GetMousePos();
		wstring str = std::format(L"mouse Position = X:({0}), Y:({1})", mousePos.x, mousePos.y);
		::TextOut(_hdc, 200, 10, str.c_str(), static_cast<int>(str.size()));
	}

	GET_SINGLE(SceneManager).Render(_hdc);
}