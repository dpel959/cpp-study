#include "pch.h"
#include "FortressScene.h"
#include "UIManager.h"
#include "Player.h"
#include "Bullet.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include <algorithm>

namespace
{
	RECT GetMiniMapContentRect()
	{
		return RECT{
			GWinSizeX - GMinimapSizeX - GMinimapMargin + 5,
			GMinimapMargin + GMinimapTitleHeight,
			GWinSizeX - GMinimapMargin - 5,
			GMinimapMargin + GMinimapSizeY - 5
		};
	}

	POINT WorldToMiniMap(Pos worldPos, const RECT& rect)
	{
		const float xRatio = std::clamp(worldPos.x / GWinSizeX, 0.f, 1.f);
		const float yRatio = std::clamp(worldPos.y / GWinSizeY, 0.f, 1.f);
		return POINT{
			rect.left + static_cast<LONG>((rect.right - rect.left) * xRatio),
			rect.top + static_cast<LONG>((rect.bottom - rect.top) * yRatio)
		};
	}

	void DrawMiniMapMarker(HDC hdc, POINT position, COLORREF color, int32 radius)
	{
		const COLORREF oldBrushColor = ::SetDCBrushColor(hdc, color);
		::Ellipse(
			hdc,
			position.x - radius,
			position.y - radius,
			position.x + radius,
			position.y + radius);
		::SetDCBrushColor(hdc, oldBrushColor);
	}
}

void FortressScene::Init()
{
	GET_SINGLE(UIManager).Init();
	_terrain.Init();
	_playerTurn = 1;
	_sumTime = 0.f;
	_winnerPlayerId = -1;

	_players[0] = GET_SINGLE(ObjectManager).CreateObject<Player>();
	_players[0]->SetTerrain(&_terrain);
	_players[0]->SetPlayerType(PlayerType::MissileTank);
	_players[0]->SetPos(Vector{ 100.f, 0.f });
	_players[0]->SetDir(Dir::Right);
	_players[0]->SetPlayerId(0);
	_players[0]->SetPlayerTurn(false);
	_players[0]->SnapToGround();

	_players[1] = GET_SINGLE(ObjectManager).CreateObject<Player>();
	_players[1]->SetTerrain(&_terrain);
	_players[1]->SetPlayerType(PlayerType::CanonTank);
	_players[1]->SetPos(Vector{ 700.f, 0.f });
	_players[1]->SetDir(Dir::Left);
	_players[1]->SetPlayerId(1);
	_players[1]->SetPlayerTurn(false);
	_players[1]->SnapToGround();

	ChangePlayerTurn();
}

void FortressScene::Update()
{
	if (_winnerPlayerId >= 0)
	{
		if (GET_SINGLE(InputManager).GetButtonDown(KeyType::R))
		{
			GET_SINGLE(SceneManager).RestartCurrentScene();
		}
		return;
	}

	GET_SINGLE(ObjectManager).Update();

	// 발사 후에는 포탄이 사라질 때까지 턴 타이머를 멈춘다.
	if (_players[_playerTurn]->GetPlayerTurn() == false)
	{
		return;
	}

	_sumTime += GET_SINGLE(TimeManager).GetDeltaTime();
	while (_sumTime >= 1.f)
	{
		_sumTime -= 1.f;

		const int32 remainTime = std::max(0, GET_SINGLE(UIManager).GetRemainTime() - 1);
		GET_SINGLE(UIManager).SetRemainTime(remainTime);

		if (remainTime == 0)
		{
			ChangePlayerTurn();
			break;
		}
	}
}

void FortressScene::Render(HDC hdc)
{
	_terrain.Render(hdc);
	GET_SINGLE(ObjectManager).Render(hdc);
	GET_SINGLE(UIManager).Render(hdc);
	RenderMiniMap(hdc);
	RenderGameResult(hdc);
}

bool FortressScene::ResolveBullet(Bullet* bullet)
{
	if (bullet == nullptr)
	{
		return false;
	}

	const Pos bulletPos = bullet->GetPos();
	const float bulletRadius = bullet->GetRadius();
	const bool isOutsideScreen =
		bulletPos.x < -bulletRadius || bulletPos.x > GWinSizeX + bulletRadius ||
		bulletPos.y < -bulletRadius || bulletPos.y > GWinSizeY + bulletRadius;

	if (isOutsideScreen)
	{
		FinishShot(bullet, true);
		return true;
	}

	// 탱크는 지면과 맞닿아 있으므로, 지형보다 탱크 충돌을 먼저 판정한다.
	for (Player* player : _players)
	{
		if (player == nullptr || player->GetPlayerId() == bullet->GetOwnerId())
		{
			continue;
		}

		const Vector distance = player->GetPos() - bulletPos;
		const float collisionRadius = player->GetRadius() + bulletRadius;
		if (distance.LengthSquared() > collisionRadius * collisionRadius)
		{
			continue;
		}

		player->TakeDamage(GBulletDamage);
		if (player->IsDead())
		{
			_winnerPlayerId = bullet->GetOwnerId();
			for (Player* turnPlayer : _players)
			{
				turnPlayer->SetPlayerTurn(false);
			}
			FinishShot(bullet, false);
		}
		else
		{
			FinishShot(bullet, true);
		}
		return true;
	}

	if (_terrain.ContainsX(bulletPos.x) &&
		bulletPos.y + bulletRadius >= _terrain.GetGroundY(bulletPos.x))
	{
		FinishShot(bullet, true);
		return true;
	}

	return false;
}

void FortressScene::ChangePlayerTurn()
{
	_playerTurn = (_playerTurn + 1) % static_cast<int32>(_players.size());

	for (Player* player : _players)
	{
		player->SetPlayerTurn(player->GetPlayerId() == _playerTurn);
	}

	_sumTime = 0.f;
	GET_SINGLE(UIManager).SetRemainTime(GTurnDuration);
	GET_SINGLE(UIManager).SetStaminaPercent(100.f);
	GET_SINGLE(UIManager).SetPowerPercent(0.f);
	GET_SINGLE(UIManager).SetWindPercent(RandomUtils::GetRandomFloat(-100.f, 100.f));
}

void FortressScene::FinishShot(Bullet* bullet, bool changeTurn)
{
	GET_SINGLE(ObjectManager).Remove(bullet);
	if (changeTurn)
	{
		ChangePlayerTurn();
	}
}

void FortressScene::RenderMiniMap(HDC hdc) const
{
	const RECT mapRect = GetMiniMapContentRect();
	_terrain.RenderMiniMap(hdc, mapRect);

	HBRUSH stockBrush = static_cast<HBRUSH>(::GetStockObject(DC_BRUSH));
	HPEN stockPen = static_cast<HPEN>(::GetStockObject(DC_PEN));
	HBRUSH oldBrush = static_cast<HBRUSH>(::SelectObject(hdc, stockBrush));
	HPEN oldPen = static_cast<HPEN>(::SelectObject(hdc, stockPen));
	const COLORREF oldPenColor = ::SetDCPenColor(hdc, RGB(20, 25, 32));

	for (Player* player : _players)
	{
		const COLORREF color = player->GetPlayerId() == 0
			? RGB(70, 190, 225)
			: RGB(245, 173, 66);
		DrawMiniMapMarker(hdc, WorldToMiniMap(player->GetPos(), mapRect), color, 3);
	}

	for (Object* object : GET_SINGLE(ObjectManager).GetObjects())
	{
		if (object->GetObjectType() == ObjectType::Projectile)
		{
			DrawMiniMapMarker(
				hdc,
				WorldToMiniMap(object->GetPos(), mapRect),
				RGB(245, 245, 245),
				2);
		}
	}

	::SetDCPenColor(hdc, oldPenColor);
	::SelectObject(hdc, oldPen);
	::SelectObject(hdc, oldBrush);
}

void FortressScene::RenderGameResult(HDC hdc) const
{
	if (_winnerPlayerId < 0)
	{
		return;
	}

	const RECT resultRect = { 220, 190, 580, 310 };
	HBRUSH stockBrush = static_cast<HBRUSH>(::GetStockObject(DC_BRUSH));
	HBRUSH oldBrush = static_cast<HBRUSH>(::SelectObject(hdc, stockBrush));
	const COLORREF oldBrushColor = ::SetDCBrushColor(hdc, RGB(35, 42, 52));
	::Rectangle(hdc, resultRect.left, resultRect.top, resultRect.right, resultRect.bottom);

	const int32 oldBackgroundMode = ::SetBkMode(hdc, TRANSPARENT);
	const COLORREF oldTextColor = ::SetTextColor(hdc, RGB(238, 242, 247));
	RECT textRect = resultRect;
	const std::wstring message = std::format(
		L"PLAYER {} WINS!\n\n[R] RESTART",
		_winnerPlayerId + 1);
	::DrawTextW(
		hdc,
		message.c_str(),
		static_cast<int32>(message.size()),
		&textRect,
		DT_CENTER | DT_VCENTER | DT_WORDBREAK);

	::SetTextColor(hdc, oldTextColor);
	::SetBkMode(hdc, oldBackgroundMode);
	::SetDCBrushColor(hdc, oldBrushColor);
	::SelectObject(hdc, oldBrush);
}
