#pragma once
#include "Object.h"

class Terrain;

//원래 상속은 Creature / Projectile로 한번 더 나누는 게 좋지만 지금은 편의상 다 Object로.
class Player : public Object
{
public:
	Player();
	virtual ~Player() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	std::wstring GetMeshKey() const; // 리소스 판별용
	void TakeDamage(int32 damage);
	void SnapToGround();
	
	void SetTerrain(const Terrain* terrain) { _terrain = terrain; }
	void SetPlayerType(PlayerType playerType) { _playerType = playerType; }
	void SetPlayerId(int32 playerId) { _playerId = playerId; }
	void SetPlayerTurn(bool playerTurn) { _playerTurn = playerTurn;}

	int32 GetPlayerId() const { return _playerId; }
	PlayerType GetPlayerType() const { return _playerType; }
	bool GetPlayerTurn() const { return _playerTurn; }
	int32 GetHp() const { return _stat.hp; }
	int32 GetMaxHp() const { return _stat.maxHp; }
	bool IsDead() const { return _stat.hp <= 0; }

	void UpdateFireAngle();

public:
	int32 _playerId = 0;
	bool _playerTurn = false;
	float _fireAngle = 0.f;
	PlayerType _playerType = PlayerType::CanonTank;
	const Terrain* _terrain = nullptr;
	HBRUSH _turnBrush = nullptr;
};
