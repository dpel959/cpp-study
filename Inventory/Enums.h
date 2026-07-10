#pragma once
#include <cstdint>

// *************************
// enum은 크지 않을때가 많으므로 크기 조절을 해주면 좋다.
// *************************

enum class EItemRarity : uint8_t
{
	None,
	Normal,
	Rare,
	Unique
};

enum class EItemType : uint8_t
{
	None,
	Weapon,
	Armor,
	Consumable
};