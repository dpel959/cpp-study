#pragma once

class UIManager
{
	DECLARE_SINGLE(UIManager);

public:
	~UIManager();

	void Init();
	void Render(HDC hdc) const;

	void SetWindPercent(float windPercent);
	void SetPowerPercent(float powerPercent);
	void SetStaminaPercent(float staminaPercent);
	void SetPlayerAngle(float angle) { _playerAngle = angle; }
	void SetBarrelAngle(float angle) { _barrelAngle = angle; }
	void SetSpecialWeapon(bool specialWeapon) { _specialWeapon = specialWeapon; }
	void SetRemainTime(int32 remainTime);

	float GetWindPercent() const { return _windPercent; }
	float GetPowerPercent() const { return _powerPercent; }
	float GetStaminaPercent() const { return _staminaPercent; }
	float GetPlayerAngle() const { return _playerAngle; }
	float GetBarrelAngle() const { return _barrelAngle; }
	bool GetSpecialWeapon() const { return _specialWeapon; }
	int32 GetRemainTime() const { return _remainTime; }

private:
	void CreateGdiResources();
	void ClearGdiResources();
	void RenderBackground(HDC hdc) const;
	void RenderWind(HDC hdc) const;
	void RenderPower(HDC hdc) const;
	void RenderStamina(HDC hdc) const;
	void RenderTime(HDC hdc) const;
	void RenderAngle(HDC hdc) const;
	void RenderWeaponChoice(HDC hdc) const;
	void RenderMiniMap(HDC hdc) const;

private:
	float _windPercent = 0.f;
	float _powerPercent = 0.f;
	float _staminaPercent = 0.f;
	float _playerAngle = 0.f;
	float _barrelAngle = 0.f;
	bool _specialWeapon = false;
	int32 _remainTime = 0;

	// 강의와 차별점 : 매 프레임 만들던 GDI 객체를 초기화 시 한 번 생성해 재사용한다.
	HBRUSH _panelBrush = nullptr;
	HBRUSH _trackBrush = nullptr;
	HBRUSH _windBrush = nullptr;
	HBRUSH _powerBrush = nullptr;
	HBRUSH _staminaBrush = nullptr;
	HPEN _borderPen = nullptr;
	HPEN _centerPen = nullptr;
	HPEN _playerAnglePen = nullptr;
	HPEN _barrelAnglePen = nullptr;
};
