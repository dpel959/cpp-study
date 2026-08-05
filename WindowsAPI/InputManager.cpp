#include "pch.h"
#include "InputManager.h"

// 이거는 왜 핸들 번호 받아요? -> 타임 매니저는 진짜 시간만 구해주면 됐는데, 이건 그렇지는 않아서.
void InputManager::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_states.resize(KEY_TYPE_COUNT, KeyState::None);
}

// 현재 키보드 상태 긁어오기
void InputManager::Update()
{
	// 현재 키 값이 눌려있는 지 확인. 근데 이걸 계속 써서 받아오면 속도가 느려짐.
	//::GetAsyncKeyState()

	// 위에건 키 하나 하나를 보는 건데, 이건 한 번에 전체 상태를 쫙 긁어오는 거임.
	// (근데 그러면 이게 더 무겁지 않아? -> 실시간으로 긁어오는 게 아니라, 이건 매 프레임마다 모두 가져오는 거라 괜찮다고 함. 음.,.?)
	
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	if (::GetKeyboardState(asciiKeys) == false) // 이게 뭐야? 배열 자체를 왜 넣어? 배열에 모든 상태를 긁어와주는거야?
	{
		return;
	}

	for (uint32 key = 0; key < KEY_TYPE_COUNT; ++key)
	{
		// 0x80이랑 비트 &해서 맞으면 눌린 것
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = _states[key];

			if (state == KeyState::Press || state == KeyState::Down)
			{
				// 눌리고 있는 중이에요 (이전 프레임에 이미 눌림)
				state = KeyState::Press;
			}
			else
			{
				// 지금 눌렸어요
				state = KeyState::Down;
			}
		}
		else
		{
			KeyState& state = _states[key];

			if (state == KeyState::Press || state == KeyState::Down)
			{
				// 눌렸는데 떼졌어요
				state = KeyState::Up;
			}
			else
			{
				// 원래 아무것도 안 하고 있었는데요
				state = KeyState::None;
			}
		}
	}

	::GetCursorPos(&_mousePos); // 커서의 좌표 알려줌
	::ScreenToClient(_hwnd, &_mousePos); // 근데 그 좌표를 우리 창 기준으로 한 포지션으로 바꿔줌.
} 