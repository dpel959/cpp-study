#include "pch.h"
#include "Terrain.h"
#include <algorithm>
#include <cmath>

Terrain::~Terrain()
{
	ClearGdiResources();
}

void Terrain::Init()
{
	ClearGdiResources();

	_heightByX.resize(GWinSizeX);
	for (int32 x = 0; x < GWinSizeX; ++x)
	{
		const float largeHill = 32.f * std::sin(x * 0.011f);
		const float smallHill = 14.f * std::sin(x * 0.029f + 1.2f);
		const float groundY = 420.f + largeHill + smallHill;
		_heightByX[x] = static_cast<int32>(std::clamp(groundY, 340.f, 450.f));
	}

	_groundPolygon.clear();
	_groundPolygon.reserve(_heightByX.size() + 2);
	for (int32 x = 0; x < GWinSizeX; ++x)
	{
		_groundPolygon.push_back(POINT{ x, _heightByX[x] });
	}
	_groundPolygon.push_back(POINT{ GWinSizeX - 1, GWinSizeY });
	_groundPolygon.push_back(POINT{ 0, GWinSizeY });

	_groundBrush = ::CreateSolidBrush(RGB(107, 142, 82));
	_groundPen = ::CreatePen(PS_SOLID, 2, RGB(67, 95, 55));
	_miniMapPen = ::CreatePen(PS_SOLID, 1, RGB(145, 190, 115));
}

void Terrain::Render(HDC hdc) const
{
	if (_groundPolygon.empty() || _groundBrush == nullptr || _groundPen == nullptr)
	{
		return;
	}

	HBRUSH oldBrush = static_cast<HBRUSH>(::SelectObject(hdc, _groundBrush));
	HPEN oldPen = static_cast<HPEN>(::SelectObject(hdc, _groundPen));
	::Polygon(hdc, _groundPolygon.data(), static_cast<int32>(_groundPolygon.size()));
	::SelectObject(hdc, oldPen);
	::SelectObject(hdc, oldBrush);
}

void Terrain::RenderMiniMap(HDC hdc, const RECT& rect) const
{
	if (_heightByX.empty() || _miniMapPen == nullptr)
	{
		return;
	}

	const LONG width = rect.right - rect.left;
	const LONG height = rect.bottom - rect.top;
	std::vector<POINT> points;
	points.reserve(width + 1);

	for (LONG miniX = 0; miniX <= width; ++miniX)
	{
		const float worldRatio = static_cast<float>(miniX) / static_cast<float>(width);
		const int32 worldX = std::min(
			GWinSizeX - 1,
			static_cast<int32>(worldRatio * GWinSizeX));
		const LONG miniY = rect.top + static_cast<LONG>(
			static_cast<float>(_heightByX[worldX]) / GWinSizeY * height);
		points.push_back(POINT{ rect.left + miniX, miniY });
	}

	HPEN oldPen = static_cast<HPEN>(::SelectObject(hdc, _miniMapPen));
	::Polyline(hdc, points.data(), static_cast<int32>(points.size()));
	::SelectObject(hdc, oldPen);
}

float Terrain::GetGroundY(float x) const
{
	if (_heightByX.empty())
	{
		return static_cast<float>(GWinSizeY);
	}

	const int32 index = std::clamp(static_cast<int32>(x), 0, GWinSizeX - 1);
	return static_cast<float>(_heightByX[index]);
}

bool Terrain::ContainsX(float x) const
{
	return x >= 0.f && x < static_cast<float>(GWinSizeX);
}

void Terrain::ClearGdiResources()
{
	if (_groundBrush != nullptr)
	{
		::DeleteObject(_groundBrush);
		_groundBrush = nullptr;
	}

	if (_groundPen != nullptr)
	{
		::DeleteObject(_groundPen);
		_groundPen = nullptr;
	}

	if (_miniMapPen != nullptr)
	{
		::DeleteObject(_miniMapPen);
		_miniMapPen = nullptr;
	}
}
