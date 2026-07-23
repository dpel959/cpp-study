#include "pch.h"
#include "Board.h"
#include "Player.h"
#include "ConsoleHelper.h"
#include "RandomUtils.h"


const char* TILE_CHAR = "■";

// 강의와 차별점 : 왜 굳이 의존성이 없는데 Init 함수를 써야하지?를 모르겠어서, 
// 그렇다면 size가 3보다 낮으면 return 해버리는 코드를 만들었음.
// 이러면 생성자로 하면 위험하므로, (다시 안전히 구축될 수 없음) Init 함수로 하는 이유가 생김.
// 및, enterPos와 exitPos의 입력 자유를 주었고, 그것을 위해 Optional을 사용. (매개변수 디폴트 값에 매개변수를 사용할 수 없기에)

void Board::Init(int32 size, Player* player, Pos enterPos, std::optional<Pos> exitPos)
{
	if (size < 3)
	{
		cout << "Board는 3 이상의 사이즈여야합니다.\n";
		return;
	}
	else if (player == nullptr)
	{
		cout << "Player가 nullptr 입니다.\n";
		return;
	}

	_size = size;

	_player = player;

	_enterPos = enterPos;

	_exitPos = exitPos.value_or(Pos(size - 2, size - 2));

	GenerateMap();
}

void Board::Render()
{
	ConsoleHelper::SetCursorPosition(0, 0);
	ConsoleHelper::ShowConsoleCursor(false);

	for (int32 y = 0; y < 25; ++y)
	{
		for (int32 x = 0; x < 25; ++x)
		{
			ConsoleColor color = GetTileColor(Pos(y, x));
			ConsoleHelper::SetCursorColor(color);
			cout << TILE_CHAR;
		}
		cout << '\n';
	}
}

// Binary Tree 미로 생성 알고리즘
// 엄청 단순함. 그냥 1/2 확률로 어느 쪽으로 뚫어주냐임
void Board::GenerateMap()
{
	for (int32 y = 0; y < _size; ++y)
	{
		for (int32 x = 0; x < _size; ++x)
		{
			if (x % 2 == 0 || y % 2 == 0)
			{
				_tile[y][x] = TileType::WALL;
			}
			else
			{
				_tile[y][x] = TileType::EMPTY;
			}
		}
	}

	// 랜덤으로 우측 or 아래로 길을 뚫는 작업.
	// 강의와 차별점 : 따로 변수로 저장해 O^2에서의 참조 줄임

	int exitY = _exitPos.y;
	int exitX = _exitPos.x;

	for (int32 y = 0; y < _size; ++y)
	{
		for (int32 x = 0; x < _size; ++x)
		{
			// 벽이므로 스킵
			if (x % 2 == 0 || y % 2 == 0)
			{
				continue;
			}

			// 목표지점. 스킵
			if (y == exitY && x  == exitX)
			{
				continue;
			}

			// 하단 도달 : 우측으로 뚫음

			if (y == exitY)
			{
				_tile[y][x + 1] = TileType::EMPTY;
				continue;
			}

			// 우측 도달 : 하단으로 뚫음.

			if (x == exitX)
			{
				_tile[y + 1][x] = TileType::EMPTY;
				continue;
			}

			// 일반적인 상황 : 우측이나 하단. 랜덤

			int randomNum = Random::GetRandomInt(0, 1);
			if (randomNum == 0)
			{
				_tile[y][x + 1] = TileType::EMPTY;
			}
			else
			{
				_tile[y + 1][x] = TileType::EMPTY;
			}
		}
	}
}

TileType Board::GetTileType(Pos pos)
{
	if (pos.x < 0 || pos.x >= _size)
	{
		return TileType::NONE;
	}
	if (pos.y < 0 || pos.y >= _size)
	{
		return TileType::NONE;
	}

	return _tile[pos.y][pos.x];
}

ConsoleColor Board::GetTileColor(Pos pos)
{
	if (_player && _player->GetPos() == pos)
		return ConsoleColor::YELLOW;

	if (GetExitPos() == pos)
	{
		return ConsoleColor::BLUE;
	}

	TileType tileType = GetTileType(pos);

	switch (tileType)
	{
	case TileType::EMPTY:
		return ConsoleColor::GREEN;
	case TileType::WALL:
		return ConsoleColor::RED;
	}

	return ConsoleColor::WHITE;
}