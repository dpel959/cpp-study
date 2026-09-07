#pragma once
#include "Vector.h"

// 헤더에 헤더를 넣는 건 웬만하면 지양해야하지만, 크기를 알아야 함

class Board;

struct PathfindingResult
{
	bool found = false;
	int32 pathCost = 0;
	int32 moveCount = 0;
	int32 expandedNodes = 0;
	int32 enqueuedNodes = 0;
	int32 maxOpenSize = 0;
	int64 elapsedNanoseconds = 0;
};

class Player
{
	static constexpr int MOVE_TICK = 100;

public:
	void Init(Board* board);
	void Update(uint64 deltaTick);
	void PrintPathfindingComparison() const;

	void SetPos(Pos pos) { _pos = pos; }
	Pos GetPos() const { return _pos; }
	const PathfindingResult& GetBfsResult() const { return _bfsResult; }
	const PathfindingResult& GetDijkstraResult() const { return _dijkstraResult; }
	const PathfindingResult& GetClosedAStarResult() const { return _closedAStarResult; }
	const PathfindingResult& GetNoClosedAStarResult() const { return _noClosedAStarResult; }

	bool CanGo(Pos pos) const;

private:
	void CalculatePath_RightHand();
	PathfindingResult CalculatePath_BFS();
	PathfindingResult CalculatePath_Dijkstra();
	PathfindingResult CalculatePath_AStar();
	PathfindingResult CalculatePath_NoClosedAStar();
	void RunPathfindingComparison();

private:
	Pos _pos;
	int32 _dir = Dir::UP;
	Board* _board = nullptr;

	vector<Pos> _path;
	int32 _pathIndex = 0;
	uint64 _sumTick = 0;

	PathfindingResult _bfsResult;
	PathfindingResult _dijkstraResult;
	PathfindingResult _closedAStarResult;
	PathfindingResult _noClosedAStarResult;
};

// 왜 Player에 Board가 있나요? -> 알고리즘 때문에 board에 벽이 있는지 없는지는 알아야 함.
