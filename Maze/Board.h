#pragma once

constexpr int BOARD_MAX_SIZE = 100;

enum class TileType : uint8
{
	NONE,
	EMPTY,
	WALL
};

enum class ConsoleColor : uint8; // 전방선언

class Player; // 전방선언

class Board
{
public:
	Board();
	~Board();

	void Init(int32 size, Player* player);
	void Render();
	
	void GenerateMap();
	TileType GetTileType(Pos pos);
	ConsoleColor GetTileColor(Pos pos);

	Pos GetEnterPos();
	Pos GetExitPos();

private:
	TileType _tile[BOARD_MAX_SIZE][BOARD_MAX_SIZE];
	int32 _size = 0;
	Pos _exitPos = Pos(0, 0);
	Player* _player = nullptr;
};