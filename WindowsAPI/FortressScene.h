#pragma once
#include "Scene.h"
#include "Terrain.h"
#include <array>

class Bullet;
class Player;

class FortressScene : public Scene
{
public:
	FortressScene() = default;
	virtual ~FortressScene() override = default;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	// 포탄 이동 직후 호출한다. 충돌이나 화면 이탈로 비행이 끝났다면 true를 반환한다.
	bool ResolveBullet(Bullet* bullet);

private:
	void ChangePlayerTurn();
	void FinishShot(Bullet* bullet, bool changeTurn);
	void RenderMiniMap(HDC hdc) const;
	void RenderGameResult(HDC hdc) const;

private:
	Terrain _terrain;
	std::array<Player*, 2> _players = {};
	int32 _playerTurn = 1;
	float _sumTime = 0.f;
	int32 _winnerPlayerId = -1;
};
