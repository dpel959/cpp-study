#pragma once
#include <optional>
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

	void Init(int32 size, Player* player, Pos enterPos = Pos(1, 1), std::optional<Pos> exitPos = std::nullopt);
	void Render();
	
	void GenerateMap();
	TileType GetTileType(Pos pos);
	ConsoleColor GetTileColor(Pos pos);

	Pos GetEnterPos() { return _enterPos; }
	Pos GetExitPos() { return _exitPos; }

	int GetSize() const { return _size; }

private:
	TileType _tile[BOARD_MAX_SIZE][BOARD_MAX_SIZE];
	int32 _size;
	Pos _enterPos;
	Pos _exitPos;
	Player* _player = nullptr;
};