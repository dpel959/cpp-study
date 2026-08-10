#pragma once
#include "ResourceBase.h"

class LineMesh : public ResourceBase
{
public:
	void Save(std::wstring path);
	void Load(std::wstring path);

	void Render(HDC hdc, Pos pos) const;

protected:
	std::vector<std::pair<POINT, POINT>> _lines;
};