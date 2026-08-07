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
	GET_SINGLE(SceneManager).Clear();
}

void Game::Init(HWND hwnd)
{
	// device context는 안 긁어와도 된다.
	// 윈도우 핸들 번호만 있으면 찾아줌.

	_hwnd = hwnd;
	_hdc = ::GetDC(hwnd); 

	::GetClientRect(hwnd, &_rect);

	_hdcBack = ::CreateCompatibleDC(_hdc); // _hdc와 호환되는 DC를 생성. 그냥 똑같은 애를 만든다
	// 위에만 해도 비트맵은 나오는데, 우리가 원하지 않는 아주 작은 1,1 사이즈 비트맵으로 생성 될거임.
	_bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom); // 비트맵을 생성. _hdc와 호환되는 비트맵 생성.

	// 우리가 원래는 hdc에 있던 걸 바로 그려줬지만, 이번에는 버퍼에 저장해야하기에 비트맵이란 개념이 등장.
	// 비트맵은 텍스처처럼 정보를 저장하는 존재.

	// DC와 BMP를 연결. 리턴은 이전 쓰던 비트맵
	HBITMAP prev = static_cast<HBITMAP>(::SelectObject(_hdcBack, _bmpBack)); 
	::DeleteObject(prev); // 이전에 쓰던 건 필요 없으니 잘라줌.

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
		::TextOut(_hdcBack, 650, 10, str.c_str(), static_cast<int>(str.size()));

		//::Rectangle(_hdc, 200, 200, 400, 400);
	}


	{
		POINT mousePos = GET_SINGLE(InputManager).GetMousePos();
		wstring str = std::format(L"mouse Position = X:({0}), Y:({1})", mousePos.x, mousePos.y);
		::TextOut(_hdcBack, 200, 10, str.c_str(), static_cast<int>(str.size()));
	}

	GET_SINGLE(SceneManager).Render(_hdcBack);

	// 그리는 걸 전부 _hdcback에다가 해줌. 여기까지 하면 아무것도 안 뜸.

	// 그 그려진 상태를 전부 통으로 복사.
	// 비트 블리트. 고속 복사이고 _hdcBack을 _hdc로 복사해! 이거임.
	// = 더블 버퍼링.
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY);
	// 근데 여기까지 하면 배경이 검은색이 됨
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS); // _hdcBack을 하얀색으로 밀어줌
}