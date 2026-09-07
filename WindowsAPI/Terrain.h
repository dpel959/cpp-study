#pragma once

class Terrain
{
public:
	Terrain() = default;
	~Terrain();

	Terrain(const Terrain&) = delete;
	Terrain& operator=(const Terrain&) = delete;

	void Init();
	void Render(HDC hdc) const;
	void RenderMiniMap(HDC hdc, const RECT& rect) const;

	float GetGroundY(float x) const;
	bool ContainsX(float x) const;

private:
	void ClearGdiResources();

private:
	std::vector<int32> _heightByX;
	std::vector<POINT> _groundPolygon;
	HBRUSH _groundBrush = nullptr;
	HPEN _groundPen = nullptr;
	HPEN _miniMapPen = nullptr;
};
