#pragma once
#include "Types.h"

constexpr int32 GWinSizeX = 800;
constexpr int32 GWinSizeY = 600;

constexpr int32 GMinimapSizeX = 200;
constexpr int32 GMinimapSizeY = 128;
constexpr int32 GMinimapMargin = 10;
constexpr int32 GMinimapTitleHeight = 20;

constexpr float PI = 3.1415926f;

// 포탄 물리 값은 화면 좌표계 기준이다. Win32 화면에서는 아래쪽이 +Y 방향이다.
constexpr float GBulletGravity = 980.f;
constexpr float GMaxWindAcceleration = 200.f;
constexpr float GBulletPowerToSpeed = 10.f;
constexpr float GBulletSpawnDistance = 45.f;

constexpr float GPlayerAngleSpeed = 50.f;
constexpr float GPlayerMaxFireAngle = 75.f;
constexpr float GPowerChargeSpeed = 100.f;
constexpr float GPlayerGroundOffset = 24.f;
constexpr int32 GBulletDamage = 25;
constexpr int32 GTurnDuration = 10;
