#include "pch.h"
#include "EditScene.h"
#include "InputManager.h"
#include "Utils.h"
#include <fstream>
#include <limits>
#include <algorithm>

EditScene::EditScene()
{
}

EditScene::~EditScene()
{
}

void EditScene::Init()
{
}

void EditScene::Update()
{
	if (GET_SINGLE(InputManager).GetButtonDown(KeyType::LeftMouse))
	{
		POINT mousePos = GET_SINGLE(InputManager).GetMousePos();

		// 지금 처음 위치도 없다. 내가 처음 위치다
		if (_setOrigin)
		{
			_lastPos = mousePos;
			_setOrigin = false;
		}
		else
		{
			// 강의와 차별점 : emplace_back으로 내부에서 pair 객체 만들어주는 게 나음.
			_lines.emplace_back(_lastPos, mousePos);
			_lastPos = mousePos;
		}
	}


	// 나 초기화할래
	if (GET_SINGLE(InputManager).GetButtonDown(KeyType::RightMouse))
	{
		_setOrigin = true;
	}

	// Save
	// 표준 C++, windows API 방식이 따로 있다고 한다.
	if (GET_SINGLE(InputManager).GetButtonDown(KeyType::S))
	{
		// 'output file stream' 임. 그런데 이제 파일 이름이나 내용이 wide character인
		std::wofstream file;
		file.open(L"Player.txt");

		LONG minX = std::numeric_limits<LONG>::max();
		LONG maxX = std::numeric_limits<LONG>::min();
		LONG minY = std::numeric_limits<LONG>::max();
		LONG maxY = std::numeric_limits<LONG>::min();

		for (auto& line : _lines)
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

		// 라인 개수
		file << static_cast<int32>(_lines.size()) << '\n';

		for (auto& line : _lines)
		{
			POINT from = line.first;
			from.x -= midX; // 자신의 '중앙 좌표'를 빼준다.
			from.y -= midY;

			POINT to = line.second;
			to.x -= midX;
			to.y -= midY;

			std::wstring wstr = std::format(L"({0},{1}->{2},{3})", from.x, from.y, to.x, to.y);
			file << wstr << '\n';
		}

		file.close();
	}

	// Load
	if (GET_SINGLE(InputManager).GetButtonDown(KeyType::D))
	{
		std::wifstream file;
		file.open(L"Player.txt");

		int32 count;
		file >> count;

		_lines.clear();

		int32 midX = 400;
		int32 midY = 300;

		for (int32 i = 0; i < count; ++i)
		{
			POINT pt1, pt2;

			std::wstring str;
			file >> str;

			// 여기에다가 넣어주세요~ 라는 것
			::swscanf_s(str.c_str(), L"(%d,%d->%d,%d)", &pt1.x, &pt1.y, &pt2.x, &pt2.y);

			pt1.x += midX; // 출력 시 내가 원하는 '중앙 좌표'에 놓아준다.
			pt1.y += midY;
			pt2.x += midX;
			pt2.y += midY;

			_lines.emplace_back(pt1, pt2);
			_setOrigin = true;
		}
		
		file.close();
	}
}

void EditScene::Render(HDC hdc)
{
	for (auto& line : _lines)
	{
		POINT pt1 = line.first;
		POINT pt2 = line.second;

		Pos pos1 = { static_cast<float>(pt1.x), static_cast<float>(pt1.y) };

		Pos pos2 = { static_cast<float>(pt2.x), static_cast<float>(pt2.y) };

		Utils::DrawLine(hdc, pos1, pos2);
	}
}
