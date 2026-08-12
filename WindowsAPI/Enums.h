#pragma once

enum class SceneType : uint8
{
	None,
	DevScene, // 개발용 씬
	EditScene,
	//
	MenuScene,
	FortressScene
	// 보통 이렇게 나눈다고 한다.
};

enum class PlayerType
{
	CanonTank,
	MissileTank
};

enum class Dir
{
	Left,
	Right,
};