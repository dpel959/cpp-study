#pragma once

#include "Defines.h" // 매크로 같은 것들
#include "Enums.h"
#include "Types.h"
#define NOMINMAX // 강의와 차별점: windows의 전역 min, max가 거슬림. numeric_limits와 충돌.
#include <windows.h>
#include <vector>
#include <string>
#include <format>