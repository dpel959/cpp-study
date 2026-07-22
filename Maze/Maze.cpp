#include "pch.h"
#include "Vector.h"
#include "Board.h"
#include "Player.h"

/// <summary>
/// 주소값을 넣는데 값 타입 객체를 넣는 건 위험하다.
/// 스코프에서 벗어나면 사라지기 때문에. 여기서는 아니다만 습관적으로 위험.
/// 
/// ::GetTickCount64는 정밀도가 떨어지고, Windows API라 다른 플랫폼에서는 못 쓴다.
/// 경험 삼아 쓰는 걸로.
/// </summary>

int main()
{
	Board* board = new Board();
	Player* player = new Player();

	board->Init(25, player);

	player->Init(board);

	uint64 lastTick = 0;
	
	while (true)
	{
#pragma region 프레임 관리
		const uint64 currentTick = ::GetTickCount64();
		const uint64 deltaTick = currentTick - lastTick;
		lastTick = currentTick;
#pragma endregion

		player->Update(deltaTick);

		board->Render();
	}

	delete board;
	delete player;
}