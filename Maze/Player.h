#pragma once
#include "Vector.h"

// 헤더에 헤더를 넣는 건 웬만하면 지양해야하지만, 크기를 알아야 함

class Board;

class Player
{
	static constexpr int MOVE_TICK = 100;

public:
	void Init(Board* board);
	void Update(uint64 deltaTick);

	void SetPos(Pos pos) { _pos = pos; }
	Pos GetPos() const { return _pos; }

	bool CanGo(Pos pos) const;

private:
	void CalculatePath_RightHand();
	void CalculatePath_BFS();

private:
	Pos _pos;
	int32 _dir = Dir::UP;
	Board* _board = nullptr;

	vector<Pos> _path;
	int32 _pathIndex;
	uint64 _sumTick = 0;
};

// 왜 Player에 Board가 있나요? -> 알고리즘 때문에 board에 벽이 있는지 없는지는 알아야 함.