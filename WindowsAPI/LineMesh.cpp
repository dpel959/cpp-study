#include "pch.h"
#include "LineMesh.h"
#include <fstream>

void LineMesh::Save(const std::wstring& path) const
{
	std::wofstream file(path);
	if (file.is_open() == false)
	{
		return;
	}

	file << static_cast<int32>(_lines.size()) << '\n';
	if (_lines.empty())
	{
		return;
	}

	LONG minX = std::numeric_limits<LONG>::max();
	LONG maxX = std::numeric_limits<LONG>::min();
	LONG minY = std::numeric_limits<LONG>::max();
	LONG maxY = std::numeric_limits<LONG>::min();

	for (const auto& line : _lines)
	{
		POINT from = line.first;
		POINT to = line.second;

		minX = std::min({ minX, from.x, to.x });
		maxX = std::max({ maxX, from.x, to.x });
		minY = std::min({ minY, from.y, to.y });
		maxY = std::max({ maxY, from.y, to.y });
	}

	int32 midX = (maxX + minX) / 2;
	int32 midY = (maxY + minY) / 2;

	for (const auto& line : _lines)
	{
		POINT from = line.first;
		from.x -= midX; // 자신의 '중앙 좌표'를 빼준다.
		from.y -= midY;

		POINT to = line.second;
		to.x -= midX;
		to.y -= midY;

		std::wstring wstr = std::format(L"({0},{1})->({2},{3})", from.x, from.y, to.x, to.y);
		file << wstr << '\n';
	}

}

void LineMesh::Load(const std::wstring& path)
{
	_lines.clear();
	_width = 0;
	_height = 0;

	std::wifstream file(path);
	int32 count = 0;
	if (file.is_open() == false || (file >> count).fail() || count < 0)
	{
		return;
	}

	std::vector<std::pair<POINT, POINT>> loadedLines;
	loadedLines.reserve(count);

	for (int32 i = 0; i < count; ++i)
	{
		POINT pt1 = {};
		POINT pt2 = {};

		std::wstring str;
		if ((file >> str).fail())
		{
			return;
		}

		if (::swscanf_s(str.c_str(), L"(%d,%d)->(%d,%d)", &pt1.x, &pt1.y, &pt2.x, &pt2.y) != 4)
		{
			return;
		}

		loadedLines.emplace_back(pt1, pt2);
	}

	_lines = std::move(loadedLines);
	if (_lines.empty())
	{
		return;
	}

	LONG minX = std::numeric_limits<LONG>::max();
	LONG maxX = std::numeric_limits<LONG>::min();
	LONG minY = std::numeric_limits<LONG>::max();
	LONG maxY = std::numeric_limits<LONG>::min();

	for (const auto& line : _lines)
	{
		POINT from = line.first;
		POINT to = line.second;

		minX = std::min({ minX, from.x, to.x });
		maxX = std::max({ maxX, from.x, to.x });
		minY = std::min({ minY, from.y, to.y });
		maxY = std::max({ maxY, from.y, to.y });
	}

	// 사실 이건 파일을 저장할때 추가 기입하는 게 좋긴 한데, 지금은 그냥 만들어 주는 걸로.
	_width = maxX - minX;
	_height = maxY - minY;
}

// 이 ratioX, ratioY를 -로 하면 '반전으로 뒤집을 수 있다'. 이건 꽤 자주 이용한다.
// pos.x, pos.y에까지 곱하면 안 된다! 그건 기준점까지 배율을 곱해버리는 것이다.
void LineMesh::Render(HDC hdc, Pos pos, float ratioX, float ratioY) const
{
	for (auto& line : _lines)
	{
		POINT pt1 = line.first;
		POINT pt2 = line.second;

		Pos pos1 = { static_cast<float>(pt1.x * ratioX + pos.x) , static_cast<float>(pt1.y * ratioY + pos.y) };

		Pos pos2 = { static_cast<float>(pt2.x * ratioX + pos.x) , static_cast<float>(pt2.y * ratioY + pos.y) };

		Utils::DrawLine(hdc, pos1, pos2);
	}
}
