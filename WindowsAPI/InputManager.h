#pragma once
#include <limits>

// 윈도우의 각 키코드는 VK_~에 매핑되어있다. 심지어 각 영어 키는 아스키 코드에 맞게 매핑되어있어서, 그냥 키코드로 그 글자를 써도 됨.
// 사실 그걸 써도 되는데, 더 알기 쉽게 Wrapper 해주는 것

// 그리고 다른 OS에 따라 번호가 다르기 때문에 이렇게 해야한다고 하는데.
// 결국 이렇게 하면 윈도우 전용 키코드가 되는 거 아닌가? 흠..
enum class KeyType : uint8
{
	LeftMouse = VK_LBUTTON,
	RightMouse = VK_RBUTTON,

	Up = VK_UP,
	Down = VK_DOWN,
	Left = VK_LEFT,
	Right = VK_RIGHT,
	SpaceBar = VK_SPACE,

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D'
};

enum class KeyState : uint8
{
	None,
	Press,
	Down,
	Up,

	End
};

constexpr int KEY_TYPE_COUNT = static_cast<int32>(std::numeric_limits<uint8>::max()) + 1;
constexpr int KEY_STATE_COUNT = static_cast<int32>(KeyState::End);

class InputManager
{
	DECLARE_SINGLE(InputManager);

public:
	void Init(HWND hwnd);
	void Update(); 

	// 누르고 있을 때
	bool GetButton(KeyType key) { return GetState(key) == KeyState::Press; }
	
	// 맨 처음 눌렀을때
	bool GetButtonDown(KeyType key) { return GetState(key) == KeyState::Down; }

	// 맨 처음 눌렀다가 뗐을 때
	bool GetButtonUp(KeyType key) { return GetState(key) == KeyState::Up; }

	POINT GetMousePos() { return _mousePos; }

private:
	KeyState GetState(KeyType key) { return _states[static_cast<uint8>(key)]; }

private:
	HWND _hwnd = 0;
	std::vector<KeyState> _states;
	POINT _mousePos; // 그냥 x, y 가지는 struct임
};