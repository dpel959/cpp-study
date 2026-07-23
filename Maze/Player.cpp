#include "pch.h"
#include "Player.h"
#include "Board.h"
#include <algorithm>

void Player::Init(Board* board)
{
	_pos = board->GetEnterPos();
	_board = board;

	//CalculatePath_RightHand();
	CalculatePath_BFS();
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

void Player::CalculatePath_BFS()
{
	Pos pos = _pos;
	Pos dest = _board->GetExitPos();

	static const Pos dirTable[4] =
	{
		Pos(-1 ,0),
		Pos(0, -1),
		Pos(1 ,0),
		Pos(0, 1)
	};

	const int32 size = _board->GetSize();
	
	// 여기 discovered는 2차원 배열로 관리하고 있다. 기준은 없다. 그냥 관리하기 편한대로 만들면 된다. 
	// 그래프는 격자 같은 게 아니었고, 지금은 기존 데이터가 격자 단위이므로 이게 관리하기 훨씬 편하므로 이렇게 사용하는 것.
	
	vector<vector<bool>> discovered(size, vector<bool>(size, false)); 
	vector<vector<Pos>> parent(size, vector<Pos>(size, Pos(-1, -1))); //[y][x] 는 parent[y][x] 에 의해 발견됨. 을 저장

	queue<Pos> q;
	q.push(pos);
	discovered[pos.y][pos.x] = true;
	parent[pos.y][pos.x] = pos; // 시작점. 나에 의해 발견되었다고 해줌.

	Pos nextPos;

	while (q.empty() == false)
	{
		pos = q.front();
		q.pop();

		if (pos == dest)
		{
			break;
		}

		for (int32 dir = 0; dir < Dir::MAX_COUNT; ++dir)
		{
			nextPos = pos + dirTable[dir];
			// 안 막혔는지 + 미발견 좌표인지 확인
			if (CanGo(nextPos) && discovered[nextPos.y][nextPos.x] == false)
			{
				q.push(nextPos);
				discovered[nextPos.y][nextPos.x] = true;
				parent[nextPos.y][nextPos.x] = pos;
			}
		}
	}

	_path.clear();
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
}