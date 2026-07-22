#include "pch.h"
#include "Player.h"
#include "Board.h"

void Player::Init(Board* board)
{
	_pos = board->GetEnterPos();
	_board = board;

	CalculatePath();
}

void Player::Update(uint64 deltaTick)
{
	if (_pathIndex >= _path.size())
		return;

	_sumTick += deltaTick;

	if (_sumTick >= MOVE_TICK)
	{
		_sumTick = 0;
		_pos = _path[_pathIndex];
		++_pathIndex;
	}
}

void Player::SetPos(Pos pos)
{
	_pos = pos;
}

Pos Player::GetPos()
{
	return _pos;
}

bool Player::CanGo(Pos pos)
{
	TileType tileType = _board->GetTileType(pos);
	return tileType == TileType::EMPTY;
}

void Player::CalculatePath()
{
	Pos pos = _pos;
	Pos dest = _board->GetExitPos();

	_path.clear();
	_path.push_back(pos);

	Pos nextPos = pos;

	Pos dirTable[4] =
	{
		Pos(-1 ,0),
		Pos(0, -1),
		Pos(1 ,0),
		Pos(0, 1)
	};

	// 3 부터 6. 즉 오른쪽부터 반시계
	while (pos != dest)
	{
		for (int i = 3; i <= 6; ++i)
		{
			nextPos = pos + dirTable[(_dir + i) % 4];
			if (CanGo(nextPos))
			{
				_dir = (_dir + i) % 4; // i = 4일때는 필요 없는데, 코드 축약. 굳이 if 넣어서 분기 예측 박살낼 필요 없음.
				pos = nextPos;
				_path.push_back(pos);
				break;
			}
		}
	}
}