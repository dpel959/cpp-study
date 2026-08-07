#pragma once
class Game
{
public:
	Game();
	~Game();

public:
	// 얘네 3총사는 언제나 있을 것임
	void Init(HWND hwnd); // 근데 Init이 있어야하는 이유는 뭘까? 생성자가 있는데.
	void Update();
	void Render();

private:
	HWND _hwnd = 0;
	HDC _hdc = 0;

private:
	RECT _rect;
	HDC _hdcBack = {};
	HBITMAP _bmpBack = {};
};