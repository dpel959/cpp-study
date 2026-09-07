#include "pch.h"
#include "MenuScene.h"
#include "InputManager.h"
#include "SceneManager.h"

MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{
	if (_titleFont != nullptr)
	{
		::DeleteObject(_titleFont);
		_titleFont = nullptr;
	}

	if (_bodyFont != nullptr)
	{
		::DeleteObject(_bodyFont);
		_bodyFont = nullptr;
	}

	if (_frameBrush != nullptr)
	{
		::DeleteObject(_frameBrush);
		_frameBrush = nullptr;
	}

	if (_startBrush != nullptr)
	{
		::DeleteObject(_startBrush);
		_startBrush = nullptr;
	}
}

void MenuScene::Init()
{
	_titleFont = ::CreateFontW(
		52, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

	_bodyFont = ::CreateFontW(
		24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

	_frameBrush = ::CreateSolidBrush(RGB(35, 42, 52));
	_startBrush = ::CreateSolidBrush(RGB(70, 190, 225));
}

void MenuScene::Update()
{
	if (GET_SINGLE(InputManager).GetButtonDown(KeyType::E))
	{
		GET_SINGLE(SceneManager).ChangeScene(SceneType::FortressScene);
	}
}

void MenuScene::Render(HDC hdc)
{
	const RECT frameRect = { 120, 120, GWinSizeX - 120, GWinSizeY - 120 };
	::FillRect(hdc, &frameRect, _frameBrush);

	const int32 oldBackgroundMode = ::SetBkMode(hdc, TRANSPARENT);
	const COLORREF oldTextColor = ::SetTextColor(hdc, RGB(238, 242, 247));

	RECT titleRect = { frameRect.left, 185, frameRect.right, 260 };
	HFONT oldFont = static_cast<HFONT>(::SelectObject(hdc, _titleFont));
	constexpr wchar_t title[] = L"FORTRESS PRACTICE";
	::DrawTextW(hdc, title, -1, &titleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT subtitleRect = { frameRect.left, 270, frameRect.right, 315 };
	::SelectObject(hdc, _bodyFont);
	constexpr wchar_t subtitle[] = L"C++ / Win32 GDI";
	::DrawTextW(hdc, subtitle, -1, &subtitleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	const RECT startRect = { 290, 350, 510, 410 };
	::FillRect(hdc, &startRect, _startBrush);

	::SetTextColor(hdc, RGB(20, 25, 32));
	RECT startTextRect = startRect;
	constexpr wchar_t startText[] = L"[ E ]  START";
	::DrawTextW(hdc, startText, -1, &startTextRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	::SelectObject(hdc, oldFont);
	::SetTextColor(hdc, oldTextColor);
	::SetBkMode(hdc, oldBackgroundMode);
}
