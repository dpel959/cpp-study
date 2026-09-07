#include "pch.h"
#include "UIManager.h"
#include <algorithm>

namespace
{
	constexpr COLORREF PanelColor = RGB(35, 42, 52);
	constexpr COLORREF BorderColor = RGB(103, 116, 137);
	constexpr COLORREF TextColor = RGB(238, 242, 247);
	constexpr COLORREF TrackColor = RGB(20, 25, 32);

	void FillRectangle(HDC hdc, const RECT& rect, HBRUSH brush)
	{
		::FillRect(hdc, &rect, brush);
	}

	void DrawOutline(HDC hdc, const RECT& rect, HPEN pen)
	{
		HPEN oldPen = static_cast<HPEN>(::SelectObject(hdc, pen));
		HBRUSH oldBrush = static_cast<HBRUSH>(::SelectObject(hdc, ::GetStockObject(NULL_BRUSH)));
		::Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		::SelectObject(hdc, oldBrush);
		::SelectObject(hdc, oldPen);
	}

	void DrawPanel(HDC hdc, const RECT& rect, HBRUSH brush, HPEN pen)
	{
		FillRectangle(hdc, rect, brush);
		DrawOutline(hdc, rect, pen);
	}

	void DrawLabel(HDC hdc, const RECT& rect, const std::wstring& text, UINT format)
	{
		const int32 oldBackgroundMode = ::SetBkMode(hdc, TRANSPARENT);
		const COLORREF oldTextColor = ::SetTextColor(hdc, TextColor);
		HFONT font = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
		HFONT oldFont = static_cast<HFONT>(::SelectObject(hdc, font));

		RECT textRect = rect;
		::DrawTextW(hdc, text.c_str(), static_cast<int32>(text.size()), &textRect, format);

		::SelectObject(hdc, oldFont);
		::SetTextColor(hdc, oldTextColor);
		::SetBkMode(hdc, oldBackgroundMode);
	}

	void DrawBar(
		HDC hdc,
		const RECT& rect,
		float percent,
		HBRUSH trackBrush,
		HBRUSH fillBrush,
		HPEN borderPen)
	{
		FillRectangle(hdc, rect, trackBrush);
		DrawOutline(hdc, rect, borderPen);

		const float ratio = std::clamp(percent, 0.f, 100.f) / 100.f;
		RECT fillRect = rect;
		fillRect.left += 3;
		fillRect.top += 3;
		fillRect.right = fillRect.left + static_cast<LONG>((rect.right - rect.left - 6) * ratio);
		fillRect.bottom -= 3;

		if (fillRect.right > fillRect.left)
		{
			FillRectangle(hdc, fillRect, fillBrush);
		}
	}

	void DrawColoredLine(HDC hdc, Pos from, Pos to, HPEN pen)
	{
		HPEN oldPen = static_cast<HPEN>(::SelectObject(hdc, pen));
		Utils::DrawLine(hdc, from, to);
		::SelectObject(hdc, oldPen);
	}
}

UIManager::~UIManager()
{
	ClearGdiResources();
}

void UIManager::Init()
{
	_windPercent = 0.f;
	_powerPercent = 0.f;
	_staminaPercent = 100.f;
	_playerAngle = 0.f;
	_barrelAngle = 20.f;
	_specialWeapon = false;
	_remainTime = GTurnDuration;
	CreateGdiResources();
}

void UIManager::Render(HDC hdc) const
{
	RenderBackground(hdc);
	RenderWind(hdc);
	RenderPower(hdc);
	RenderStamina(hdc);
	RenderTime(hdc);
	RenderAngle(hdc);
	RenderWeaponChoice(hdc);
	RenderMiniMap(hdc);
}

void UIManager::SetWindPercent(float windPercent)
{
	_windPercent = std::clamp(windPercent, -100.f, 100.f);
}

void UIManager::SetPowerPercent(float powerPercent)
{
	_powerPercent = std::clamp(powerPercent, 0.f, 100.f);
}

void UIManager::SetStaminaPercent(float staminaPercent)
{
	_staminaPercent = std::clamp(staminaPercent, 0.f, 100.f);
}

void UIManager::SetRemainTime(int32 remainTime)
{
	_remainTime = std::max(0, remainTime);
}

void UIManager::CreateGdiResources()
{
	ClearGdiResources();
	_panelBrush = ::CreateSolidBrush(PanelColor);
	_trackBrush = ::CreateSolidBrush(TrackColor);
	_windBrush = ::CreateSolidBrush(RGB(70, 190, 225));
	_powerBrush = ::CreateSolidBrush(RGB(245, 173, 66));
	_staminaBrush = ::CreateSolidBrush(RGB(99, 205, 132));
	_borderPen = ::CreatePen(PS_SOLID, 1, BorderColor);
	_centerPen = ::CreatePen(PS_SOLID, 1, TextColor);
	_playerAnglePen = ::CreatePen(PS_SOLID, 2, RGB(155, 165, 180));
	_barrelAnglePen = ::CreatePen(PS_SOLID, 2, RGB(255, 205, 76));
}

void UIManager::ClearGdiResources()
{
	if (_panelBrush != nullptr) { ::DeleteObject(_panelBrush); _panelBrush = nullptr; }
	if (_trackBrush != nullptr) { ::DeleteObject(_trackBrush); _trackBrush = nullptr; }
	if (_windBrush != nullptr) { ::DeleteObject(_windBrush); _windBrush = nullptr; }
	if (_powerBrush != nullptr) { ::DeleteObject(_powerBrush); _powerBrush = nullptr; }
	if (_staminaBrush != nullptr) { ::DeleteObject(_staminaBrush); _staminaBrush = nullptr; }
	if (_borderPen != nullptr) { ::DeleteObject(_borderPen); _borderPen = nullptr; }
	if (_centerPen != nullptr) { ::DeleteObject(_centerPen); _centerPen = nullptr; }
	if (_playerAnglePen != nullptr) { ::DeleteObject(_playerAnglePen); _playerAnglePen = nullptr; }
	if (_barrelAnglePen != nullptr) { ::DeleteObject(_barrelAnglePen); _barrelAnglePen = nullptr; }
}

void UIManager::RenderBackground(HDC hdc) const
{
	DrawPanel(hdc, RECT{ 0, 470, GWinSizeX, GWinSizeY }, _panelBrush, _borderPen);
}

void UIManager::RenderWind(HDC hdc) const
{
	DrawLabel(
		hdc,
		RECT{ 125, 482, 275, 502 },
		std::format(L"WIND  {0:+.0f}", _windPercent),
		DT_CENTER | DT_SINGLELINE);

	const RECT trackRect = { 150, 507, 250, 527 };
	FillRectangle(hdc, trackRect, _trackBrush);
	DrawOutline(hdc, trackRect, _borderPen);

	const LONG centerX = (trackRect.left + trackRect.right) / 2;
	const float halfWidth = static_cast<float>(trackRect.right - trackRect.left - 6) / 2.f;
	RECT fillRect = { centerX, trackRect.top + 3, centerX, trackRect.bottom - 3 };
	const LONG windWidth = static_cast<LONG>(halfWidth * std::abs(_windPercent) / 100.f);

	if (_windPercent < 0.f)
	{
		fillRect.left -= windWidth;
	}
	else
	{
		fillRect.right += windWidth;
	}

	if (fillRect.right > fillRect.left)
	{
		FillRectangle(hdc, fillRect, _windBrush);
	}

	DrawColoredLine(
		hdc,
		Pos{ static_cast<float>(centerX), static_cast<float>(trackRect.top) },
		Pos{ static_cast<float>(centerX), static_cast<float>(trackRect.bottom) },
		_centerPen);
}

void UIManager::RenderPower(HDC hdc) const
{
	DrawLabel(
		hdc,
		RECT{ 300, 480, 620, 498 },
		std::format(L"POWER  {0:.0f}%", _powerPercent),
		DT_LEFT | DT_SINGLELINE);
	DrawBar(hdc, RECT{ 300, 500, 620, 520 }, _powerPercent, _trackBrush, _powerBrush, _borderPen);
}

void UIManager::RenderStamina(HDC hdc) const
{
	DrawLabel(
		hdc,
		RECT{ 300, 525, 620, 543 },
		std::format(L"STAMINA  {0:.0f}%", _staminaPercent),
		DT_LEFT | DT_SINGLELINE);
	DrawBar(hdc, RECT{ 300, 545, 620, 565 }, _staminaPercent, _trackBrush, _staminaBrush, _borderPen);
}

void UIManager::RenderTime(HDC hdc) const
{
	const RECT timeRect = { 680, 490, 770, 560 };
	DrawPanel(hdc, timeRect, _panelBrush, _borderPen);
	DrawLabel(
		hdc,
		timeRect,
		std::format(L"TIME\n{:02}", _remainTime),
		DT_CENTER | DT_VCENTER | DT_WORDBREAK);
}

void UIManager::RenderAngle(HDC hdc) const
{
	const Pos center = { 65.f, 525.f };
	constexpr float radius = 32.f;

	HPEN oldPen = static_cast<HPEN>(::SelectObject(hdc, _borderPen));
	HBRUSH oldBrush = static_cast<HBRUSH>(::SelectObject(hdc, ::GetStockObject(NULL_BRUSH)));
	::Ellipse(
		hdc,
		static_cast<int32>(center.x - radius),
		static_cast<int32>(center.y - radius),
		static_cast<int32>(center.x + radius),
		static_cast<int32>(center.y + radius));
	::SelectObject(hdc, oldBrush);
	::SelectObject(hdc, oldPen);

	auto anglePoint = [center](float angle, float length)
	{
		const float radian = angle * PI / 180.f;
		return Pos{
			center.x + length * std::cos(radian),
			center.y - length * std::sin(radian)
		};
	};

	DrawColoredLine(hdc, center, anglePoint(_playerAngle, 25.f), _playerAnglePen);
	DrawColoredLine(hdc, center, anglePoint(_barrelAngle, 29.f), _barrelAnglePen);
}

void UIManager::RenderWeaponChoice(HDC hdc) const
{
	DrawLabel(
		hdc,
		RECT{ 115, 545, 275, 570 },
		_specialWeapon ? L"WEAPON: SPECIAL" : L"WEAPON: NORMAL",
		DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void UIManager::RenderMiniMap(HDC hdc) const
{
	const RECT minimapRect = {
		GWinSizeX - GMinimapSizeX - GMinimapMargin,
		GMinimapMargin,
		GWinSizeX - GMinimapMargin,
		GMinimapMargin + GMinimapSizeY
	};
	DrawPanel(hdc, minimapRect, _panelBrush, _borderPen);
	DrawLabel(
		hdc,
		RECT{ minimapRect.left, minimapRect.top + 2, minimapRect.right, minimapRect.top + GMinimapTitleHeight },
		L"MINIMAP",
		DT_CENTER | DT_SINGLELINE);
}
