#include "pch.h"
#include "Player.h"
#include "Board.h"
#include "ConsoleHelper.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <queue>
using std::priority_queue, std::greater;

namespace
{
	constexpr int32 PATH_DIRECTION_COUNT = 8;
	constexpr int32 STRAIGHT_COST = 10;
	constexpr int32 DIAGONAL_COST = 14;

	const Pos PATH_DIRECTIONS[PATH_DIRECTION_COUNT]
	{
		Pos(-1, 0),
		Pos(0, -1),
		Pos(1, 0),
		Pos(0, 1),
		Pos(1, -1),
		Pos(1, 1),
		Pos(-1, -1),
		Pos(-1, 1)
	};

	const int32 PATH_COSTS[PATH_DIRECTION_COUNT]
	{
		STRAIGHT_COST,
		STRAIGHT_COST,
		STRAIGHT_COST,
		STRAIGHT_COST,
		DIAGONAL_COST,
		DIAGONAL_COST,
		DIAGONAL_COST,
		DIAGONAL_COST
	};

	// 강의와 차별점 : 맨해튼 거리가 아닌 Octile 거리를 이용한다.
	// 맨해튼 거리는 4방향 이동에서 판단하기 좋은 휴리스틱이지만, 8방향 이동에서는 대각선의 가중치를 수평 이동 비용의 2배로 과대평가한다.
	// 대각선의 가중치를 수평 이동의 2배 정도 까지 강하게 두고 싶지 않다(수평 10 / 대각선 14).
	// 이때, Octile 거리는 x, y로 전진할 수 있는 각 횟수를 세고, x, y가 횟수가 겹치는 만큼 대각선의 가중치로 치환한다.
	// 실제 우리가 원하는 대각선 이동 비용 14를 정확히 반영할 수 있는, 더 적합한 Octile 거리를 사용한다.
	int32 CalculateOctileDistance(Pos from, Pos to)
	{
		const int32 dx = std::abs(to.x - from.x);
		const int32 dy = std::abs(to.y - from.y);
		const int32 diagonal = (std::min)(dx, dy);
		const int32 straight = (std::max)(dx, dy) - diagonal;

		return DIAGONAL_COST * diagonal + STRAIGHT_COST * straight;
	}

	int32 CalculatePathCost(const vector<Pos>& path)
	{
		int32 pathCost = 0;

		for (int32 i = 1; i < static_cast<int32>(path.size()); ++i)
		{
			const int32 dx = std::abs(path[i].x - path[i - 1].x);
			const int32 dy = std::abs(path[i].y - path[i - 1].y);
			pathCost += (dx == 1 && dy == 1) ? DIAGONAL_COST : STRAIGHT_COST;
		}

		return pathCost;
	}

	void FinishPathfindingResult(
		PathfindingResult& result,
		const vector<Pos>& path,
		std::chrono::steady_clock::time_point startTime)
	{
		if (result.found)
		{
			result.moveCount = static_cast<int32>(path.size()) - 1;
			result.pathCost = CalculatePathCost(path);
		}

		result.elapsedNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::steady_clock::now() - startTime).count();
	}

	void PrintPathfindingResult(const char* name, const PathfindingResult& result)
	{
		cout << std::left << std::setw(16) << name
			<< std::setw(7) << (result.found ? "Yes" : "No")
			<< std::setw(7) << result.pathCost
			<< std::setw(7) << result.moveCount
			<< std::setw(10) << result.expandedNodes
			<< std::setw(10) << result.enqueuedNodes
			<< std::setw(9) << result.maxOpenSize
			<< result.elapsedNanoseconds << '\n';
	}
}

void Player::Init(Board* board)
{
	_pos = board->GetEnterPos();
	_board = board;

	RunPathfindingComparison();
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

bool Player::CanGo(Pos pos) const
{
	TileType tileType = _board->GetTileType(pos);
	return tileType == TileType::EMPTY;
}

void Player::RunPathfindingComparison()
{
	_bfsResult = CalculatePath_BFS();
	_dijkstraResult = CalculatePath_Dijkstra();
	_closedAStarResult = CalculatePath_AStar();
	_noClosedAStarResult = CalculatePath_NoClosedAStar();
}

void Player::PrintPathfindingComparison() const
{
	ConsoleHelper::SetCursorColor(ConsoleColor::WHITE);
	cout << "=== Pathfinding Comparison ===\n";
	cout << std::left << std::setw(16) << "Algorithm"
		<< std::setw(7) << "Found"
		<< std::setw(7) << "Cost"
		<< std::setw(7) << "Moves"
		<< std::setw(10) << "Expanded"
		<< std::setw(10) << "Enqueued"
		<< std::setw(9) << "MaxOpen"
		<< "Time(ns)\n";

	PrintPathfindingResult("BFS", _bfsResult);
	PrintPathfindingResult("Dijkstra", _dijkstraResult);
	PrintPathfindingResult("A* (Closed)", _closedAStarResult);
	PrintPathfindingResult("A* (No Closed)", _noClosedAStarResult);

	const bool closedCostMatched = _dijkstraResult.found && _closedAStarResult.found
		&& _dijkstraResult.pathCost == _closedAStarResult.pathCost;
	const bool noClosedCostMatched = _dijkstraResult.found && _noClosedAStarResult.found
		&& _dijkstraResult.pathCost == _noClosedAStarResult.pathCost;

	cout << "A* shortest cost validation: Closed="
		<< (closedCostMatched ? "PASS" : "FAIL")
		<< " / No Closed="
		<< (noClosedCostMatched ? "PASS" : "FAIL") << '\n';
}

void Player::CalculatePath_RightHand()
{
	Pos pos = _pos;
	Pos dest = _board->GetExitPos();

	_path.clear();
	_path.push_back(pos);

	Pos nextPos = pos;

	// 강의와 차별점. 이건 static const로 선언하자
	// 굳이 바꿀 것도 아니고, 계속 필요한데 이걸 함수 나올때마다 생성할 필요가 없다.

	static const Pos dirTable[4] =
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

PathfindingResult Player::CalculatePath_BFS()
{
	const auto startTime = std::chrono::steady_clock::now();
	PathfindingResult result;
	_path.clear();
	_pathIndex = 0;

	Pos pos = _pos;
	Pos dest = _board->GetExitPos();

	const int32 size = _board->GetSize();
	
	// 여기 discovered는 2차원 배열로 관리하고 있다. 기준은 없다. 그냥 관리하기 편한대로 만들면 된다. 
	// 그래프는 격자 같은 게 아니었고, 지금은 기존 데이터가 격자 단위이므로 이게 관리하기 훨씬 편하므로 이렇게 사용하는 것.
	
	vector<vector<bool>> discovered(size, vector<bool>(size, false)); 
	vector<vector<Pos>> parent(size, vector<Pos>(size, Pos(-1, -1))); //[y][x] 는 parent[y][x] 에 의해 발견됨. 을 저장

	queue<Pos> q;
	q.push(pos);
	result.enqueuedNodes = 1;
	result.maxOpenSize = 1;
	discovered[pos.y][pos.x] = true;
	parent[pos.y][pos.x] = pos; // 시작점. 나에 의해 발견되었다고 해줌.

	Pos nextPos;
	bool findExit = false;

	while (q.empty() == false)
	{
		pos = q.front();
		q.pop();
		++result.expandedNodes;

		if (pos == dest)
		{
			findExit = true;
			break;
		}

		for (int32 dir = 0; dir < PATH_DIRECTION_COUNT; ++dir)
		{
			nextPos = pos + PATH_DIRECTIONS[dir];
			// 안 막혔는지 + 미발견 좌표인지 확인
			if (CanGo(nextPos) && discovered[nextPos.y][nextPos.x] == false)
			{
				q.push(nextPos);
				++result.enqueuedNodes;
				result.maxOpenSize = (std::max)(
					result.maxOpenSize,
					static_cast<int32>(q.size()));
				discovered[nextPos.y][nextPos.x] = true;
				parent[nextPos.y][nextPos.x] = pos;
			}
		}
	}

	if (findExit == false)
	{
		FinishPathfindingResult(result, _path, startTime);
		return result;
	}

	pos = dest;

	// 무조건 while(true)가 나쁜 건 아니다. 완료 조건이 명확하다면.
	// 오히려 조건을 주려다가 중복 코드가 생길 수 있다.
	while(true)
	{
		_path.push_back(pos);

		nextPos = parent[pos.y][pos.x];

		if (pos == nextPos)
		{
			break;
		}

		pos = nextPos;
	}

	std::reverse(_path.begin(), _path.end());
	result.found = true;
	FinishPathfindingResult(result, _path, startTime);
	return result;
}


struct PQNode
{
	PQNode (int32 f, int32 g, Pos pos) : f(f), g(g), pos(pos) {}

	bool operator < (const PQNode& other) const { return f < other.f; }
	bool operator > (const PQNode& other) const { return f > other.f; }

	int32 f; // f = g + h
	int32 g;
	Pos pos;
};

/// 
/// A* - 목적지의 개념이 있는 다익스트라
/// 1. 입구에서 얼마나 2. 출구에서 얼마나 떨어져 있는지에 따라 점수를 줄 것.
/// F = G + H (최종 점수. 작을 수록 좋음.)
/// G = 시작점에서 해당 좌표까지 이동하는데 드는 비용
/// H = 목적지에서 해당 좌표까지 이동하는데 드는 비용
/// 
PathfindingResult Player::CalculatePath_AStar()
{
	const auto startTime = std::chrono::steady_clock::now();
	PathfindingResult result;
	_path.clear();
	_pathIndex = 0;

	Pos start = _pos;
	Pos dest = _board->GetExitPos();

	const int32 size = _board->GetSize();


	//강의와 차별점 : windows.h에 max(a,b)가 매크로가 되어있어서 numeric_limits의 max가 그냥 사용이 안됨.
	//그래서 따로 max까지를 괄호에 감싸줌. 이게 되는 이유는 max()가 아니라 (max)() 형식이라 감지를 못하기 때문.

	vector<vector<int32>> best(size, vector<int32>(size, (std::numeric_limits<int32>::max)()));

	// 이건 필요 없고 best만 있어도 판별은 되는데. 공식문서에 있으므로 일단..
	// closed list라는 것. 어떤 좌표가 방문까지 끝냈는지 여부를 따로 관리, visited임.
	// closed[y][x] -> (y,x)에 방문 했는지 여부
	// discovered와는 다름. 그건 발견했다! 이지만, 이건 발견한 게 아니고 실제로 여기가 우수하다 판단하여 방문을 한 것.
	vector<vector<bool>> closed(size, vector<bool>(size, false));

	// 부모 추적 용도
	vector<vector<Pos>> parent(size, vector<Pos>(size, Pos(-1, -1)));

	// 예약 시스템 구현 + 

	// greater 잊지 말자. 이거 안 하면 가장 가중치 높은 애부터 처리한다...

	// openlist : 지금까지 '발견된' 목록. 방문한 게 아님. 나중에 더 좋은 경로를 찾을 수 있기 때문에.
	// 그저 '현상'의 베스트 케이스를 저장한 것이라 보면 됨.

	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;
	
	// 초기값
	{
		int32 g = 0; // 출발지에서 pos까지의 이동 비용
		int32 h = CalculateOctileDistance(start, dest); // 목적지에서 pos까지의 이동 비용. 휴리스틱 이라고도 함.

		// 다익스트라랑 다른 점이 하나밖에 없음. 그냥 휴리스틱. 목적지를 고려한 것. 이게 끝. 
		// g는 원래 다익스트라도 시작점에서 점수 계산을 했었음.

		pq.push(PQNode(g + h, g, start));
		result.enqueuedNodes = 1;
		result.maxOpenSize = 1;
		best[start.y][start.x] = g + h; // 이번엔 여기 0 적으면 안 된다. 목적지의 휴리스틱도 점수이기 때문에.
		parent[start.y][start.x] = start;
	}

	// 강의와 차별점 : nextPos 밖으로 빼놔야 반복문 안에서 소멸자 안 돌아간다.
	Pos nextPos;
	bool findExit = false;

	while (pq.empty() == false)
	{
		// 가장 우수한 후보 찾기
		PQNode node = pq.top();
		pq.pop();

		// 동일한 좌표를 여러 경로로 찾는다. 근데 이때 '우수한 경로'로 우선적으로 깊게 파고듦
		// 그래서, 더 빠른 경로로 인해 이미 방문 (closed) 된 경우 스킵.
		// 다익스트라와 같음. 나보다 빠른 녀석이 있으면 애초에 나는 못 이김.

		// 근데 이게 쓸모 없다고 한 이유가, 그냥 best 값이랑 내 f 값이랑 비교하면 됨.
		// 다익스트라때랑 똑같이 하면 돼서... 굳이.

		if (closed[node.pos.y][node.pos.x])
			continue;

		++result.expandedNodes;

		// 방문

		closed[node.pos.y][node.pos.x] = true;

		// 목적지에 도착했으면 바로 종료.

		if (node.pos == dest)
		{
			findExit = true;
			break;
		}

		for (int32 dir = 0; dir < PATH_DIRECTION_COUNT; ++dir)
		{
			nextPos = node.pos + PATH_DIRECTIONS[dir];

			// 갈 수 있는 지역은 맞는지 확인
			if (CanGo(nextPos) && closed[nextPos.y][nextPos.x] == false)
			{
				int32 g = node.g + PATH_COSTS[dir]; // 시작점에서 여기까지 오는데 얼마 들어!
				int32 h = CalculateOctileDistance(nextPos, dest);
				
				// 근데 이건 왜 하지? 위의 closed에서 체크한 거 아닌가?
				// 방금 말했만 closed와 best 체크되는 건 다름. closed는 결과적으로 실제로 pq에서 나와서 방문 한거고.
				// best는 일단 pq에는 들어갔는데, 아직 나오지는 못한 거임. 말 그대로 후보군
				if (best[nextPos.y][nextPos.x] <= g + h)
					continue;

				best[nextPos.y][nextPos.x] = g + h;
				pq.push(PQNode(g + h, g, nextPos));
				++result.enqueuedNodes;
				result.maxOpenSize = (std::max)(
					result.maxOpenSize,
					static_cast<int32>(pq.size()));
				parent[nextPos.y][nextPos.x] = node.pos;
			}
		}
	}

	/// 꼭 대각선을 할 필요는 없음. 그냥 위에서 대각선 요소만 없애면 상하좌우로만 이동함.
	// 강의와 차별점 : 반복문 안에 있는 parent 참조 2번을 반복문 영향 밖에 있는 nextPos로 사용

	if (findExit == false)
	{
		FinishPathfindingResult(result, _path, startTime);
		return result;
	}

	Pos pos = dest;

	while (true)
	{
		_path.push_back(pos);

		nextPos = parent[pos.y][pos.x];

		if (pos == nextPos)
		{
			break;
		}

		pos = nextPos;
	}

	std::reverse(_path.begin(), _path.end());
	result.found = true;
	FinishPathfindingResult(result, _path, startTime);
	return result;
}

struct ASNode
{
	ASNode(int32 f, int32 g, Pos pos) : f(f), g(g), pos(pos)
	{ }

	ASNode() {}

	bool operator < (const ASNode& other) const { return f < other.f; }
	bool operator > (const ASNode& other) const { return f > other.f; }

	int32 f;
	int32 g;
	Pos pos;
};

PathfindingResult Player::CalculatePath_Dijkstra()
{
	const auto startTime = std::chrono::steady_clock::now();
	PathfindingResult result;
	_path.clear();
	_pathIndex = 0;

	Pos start = _pos;
	Pos dest = _board->GetExitPos();
	const int32 size = _board->GetSize();

	priority_queue<ASNode, vector<ASNode>, greater<ASNode>> pq;
	vector<vector<int32>> best(size, vector<int32>(size, (std::numeric_limits<int32>::max)()));
	vector<vector<Pos>> parent(size, vector<Pos>(size, Pos(-1, -1)));

	pq.push(ASNode(0, 0, start));
	result.enqueuedNodes = 1;
	result.maxOpenSize = 1;
	best[start.y][start.x] = 0;
	parent[start.y][start.x] = start;

	bool findExit = false;
	Pos nextPos;

	while (pq.empty() == false)
	{
		ASNode node = pq.top();
		pq.pop();

		if (best[node.pos.y][node.pos.x] < node.g)
			continue;

		++result.expandedNodes;

		if (node.pos == dest)
		{
			findExit = true;
			break;
		}

		for (int32 dir = 0; dir < PATH_DIRECTION_COUNT; ++dir)
		{
			nextPos = node.pos + PATH_DIRECTIONS[dir];

			if (CanGo(nextPos) == false)
				continue;

			const int32 nextCost = node.g + PATH_COSTS[dir];

			if (best[nextPos.y][nextPos.x] <= nextCost)
				continue;

			best[nextPos.y][nextPos.x] = nextCost;
			parent[nextPos.y][nextPos.x] = node.pos;
			pq.push(ASNode(nextCost, nextCost, nextPos));
			++result.enqueuedNodes;
			result.maxOpenSize = (std::max)(
				result.maxOpenSize,
				static_cast<int32>(pq.size()));
		}
	}

	if (findExit == false)
	{
		FinishPathfindingResult(result, _path, startTime);
		return result;
	}

	Pos pos = dest;

	while (true)
	{
		_path.push_back(pos);
		nextPos = parent[pos.y][pos.x];

		if (pos == nextPos)
			break;

		pos = nextPos;
	}

	std::reverse(_path.begin(), _path.end());
	result.found = true;
	FinishPathfindingResult(result, _path, startTime);
	return result;
}

/// 강의와 차별점 : closedList를 사용하지 않은 A* 구현.

PathfindingResult Player::CalculatePath_NoClosedAStar() {
	const auto startTime = std::chrono::steady_clock::now();
	PathfindingResult result;
	_path.clear();
	_pathIndex = 0;

	Pos start = _pos;
	Pos dest = _board->GetExitPos();
	int32 size = _board->GetSize();

	priority_queue<ASNode, vector<ASNode>, greater<ASNode>> pq;

	vector<vector<int32>> best(size, vector<int32>(size, (std::numeric_limits<int32>::max)()));

	vector<vector<Pos>> parent(size, vector<Pos>(size, Pos(-1, -1)));

	{
		int32 g = 0;
		int32 h = CalculateOctileDistance(start, dest);
		
		pq.push(ASNode(g + h, g, start));
		result.enqueuedNodes = 1;
		result.maxOpenSize = 1;
		best[start.y][start.x] = g + h;
		parent[start.y][start.x] = start;
	}

	ASNode node;

	Pos nextPos;

	// 강의와 차별점 : bool findExit를 추가해, 출구가 없을때를 케어. 물론 우리 맵 코드에서 그럴 일은 없지만.

	bool findExit = false;

	while (pq.empty() == false)
	{
		node = pq.top();
		pq.pop();

		if (best[node.pos.y][node.pos.x] < node.f) // 여기서 =<로 하면 자기 자신까지 포함되어서, 시작점때 바로 죽어버림
			continue;

		++result.expandedNodes;

		if (node.pos == dest)
		{
			findExit = true;
			break;
		}

		for (int32 i = 0; i < PATH_DIRECTION_COUNT; ++i)
		{
			nextPos = node.pos + PATH_DIRECTIONS[i];

			if (CanGo(nextPos) == false)
				continue;

			int32 g = node.g + PATH_COSTS[i];
			int32 h = CalculateOctileDistance(nextPos, dest);

			int32 f = g + h;

			// 여기서 >=로 하면 같은 비용의 루트가 들어가 굳이 똑같은 비용인 애들을 여러번 계산하게 됨.
			// 가중치0인 간선이 있으면 무한루프를 돌게 됨.

			if (best[nextPos.y][nextPos.x] > f)
			{
				pq.push(ASNode(f, g, nextPos));
				++result.enqueuedNodes;
				result.maxOpenSize = (std::max)(
					result.maxOpenSize,
					static_cast<int32>(pq.size()));
				best[nextPos.y][nextPos.x] = f;
				parent[nextPos.y][nextPos.x] = node.pos;
			}
		}
	}

	if (findExit == false)
	{
		FinishPathfindingResult(result, _path, startTime);
		return result;
	}

	Pos pos = dest;

	while (true)
	{
		_path.push_back(pos);

		nextPos = parent[pos.y][pos.x];

		if (pos == nextPos)
			break;

		pos = nextPos;
	}

	std::reverse(_path.begin(), _path.end());
	result.found = true;
	FinishPathfindingResult(result, _path, startTime);
	return result;
}
