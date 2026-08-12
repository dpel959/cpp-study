#pragma once
#include "Object.h"

//원래 상속은 Creature / Projectile로 한번 더 나누는 게 좋지만 지금은 편의상 다 Object로.
class Player : public Object
{
public:
	Player();
	virtual ~Player() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	std::wstring GetMeshKey(); // 리소스 판별용
	
	void SetPlayerType(PlayerType playerType) { _playerType = playerType; }
	void SetPlayerId(int32 playerId) { _playerId = playerId; }
	void SetPlayerTurn(bool playerTurn) { _playerTurn = playerTurn;}

	int32 GetPlayerId() { return _playerId; }
	PlayerType GetPlayerType() { return _playerType; }
	bool GetPlayerTurn() { return _playerTurn; }

	void UpdateFireAngle();

public:
	int32 _playerId = 0;
	bool _playerTurn = false;
	float _fireAngle = 0.f;
	PlayerType _playerType = PlayerType::CanonTank;
};