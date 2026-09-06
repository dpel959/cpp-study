#pragma once

#include <cstdint>

struct ItemHandle
{
	int slotIndex = -1;              // 아이템이 들어 있는 칸 번호
	std::uint32_t generation = 0;    // 그 칸이 다시 사용될 때마다 바뀌는 번호
};
