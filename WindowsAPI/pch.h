#pragma once


#define NOMINMAX // 강의와 차별점: windows의 전역 min, max가 거슬림. numeric_limits와 충돌.
#include <windows.h>
#include <vector>
#include <string>
#include <format>

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h> // 메모리 누수 검사 도구, 함수들

#ifdef _DEBUG // 디버그 모드일때만.
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__) // 우회된 할당기. 버그 출력창에 정확한 위치를 알 수 있음. 해제 안 된 곳의 파일과 줄 위치를 출력해줌
#endif

#include "Types.h"
#include "Enums.h"
#include "Defines.h" // 매크로 같은 것들