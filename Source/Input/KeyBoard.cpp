#include <windows.h>
#include <math.h>
#include <Xinput.h>
#include "Input/KeyBoard.h"

// 更新
void KeyBoard::Update()
{
	axisLx = axisLy = 0.0f;
	axisRx = axisRy = 0.0f;
	triggerL = triggerR = 0.0f;

	KeyBoardButton newButtonState = 0;

	// キーボードでエミュレーション
	{
		float lx = 0.0f;
		float ly = 0.0f;
		float rx = 0.0f;
		float ry = 0.0f;
		if (GetAsyncKeyState('W') & 0x8000) newButtonState |= BTN_KEYBOARD_W;
		if (GetAsyncKeyState('A') & 0x8000) newButtonState |= BTN_KEYBOARD_A;
		if (GetAsyncKeyState('S') & 0x8000) newButtonState |= BTN_KEYBOARD_S;
		if (GetAsyncKeyState('D') & 0x8000) newButtonState |= BTN_KEYBOARD_D;
		if (GetAsyncKeyState('I') & 0x8000) ry = 1.0f;
		if (GetAsyncKeyState('J') & 0x8000) rx = -1.0f;
		if (GetAsyncKeyState('K') & 0x8000) ry = -1.0f;
		if (GetAsyncKeyState('L') & 0x8000) rx = 1.0f;

		if (GetAsyncKeyState('V') & 0x8000) newButtonState |= BTN_KEYBOARD_V;
		if (GetAsyncKeyState('C') & 0x8000) newButtonState |= BTN_KEYBOARD_C;
		if (GetAsyncKeyState('F') & 0x8000) newButtonState |= BTN_KEYBOARD_F;
		if (GetAsyncKeyState('R') & 0x8000) newButtonState |= BTN_KEYBOARD_R;
		if (GetAsyncKeyState('P') & 0x8000) newButtonState |= BTN_KEYBOARD_P;
		if (GetAsyncKeyState(' ') & 0x8000) newButtonState |= BTN_KEYBOARD_SPACE;
		if (GetAsyncKeyState('E') & 0x8000) newButtonState |= BTN_KEYBOARD_E;
		if (GetAsyncKeyState(VK_TAB) & 0x8000) newButtonState |= BTN_KEYBOARD_TAB;
		if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) newButtonState |= BTN_KEYBOARD_SHIFT;
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) newButtonState |= BTN_KEYBOARD_ESC;

#if 1
		if (newButtonState & BTN_KEYBOARD_W)    ly = 1.0f;
		if (newButtonState & BTN_KEYBOARD_A) lx = 1.0f;
		if (newButtonState & BTN_KEYBOARD_S)  ly = -1.0f;
		if (newButtonState & BTN_KEYBOARD_D)  lx = -1.0f;
#endif

		if (lx >= 1.0f || lx <= -1.0f || ly >= 1.0f || ly <= -1.0)
		{
			float power = ::sqrtf(lx * lx + ly * ly);
			axisLx = lx / power;
			axisLy = ly / power;
		}

		if (rx >= 1.0f || rx <= -1.0f || ry >= 1.0f || ry <= -1.0)
		{
			float power = ::sqrtf(rx * rx + ry * ry);
			axisRx = rx / power;
			axisRy = ry / power;
		}
	}



	// ボタン情報の更新
	{
		buttonState[1] = buttonState[0];	// スイッチ履歴
		buttonState[0] = newButtonState;

		buttonDown = ~buttonState[1] & newButtonState;	// 押した瞬間
		buttonUp = ~newButtonState & buttonState[1];	// 離した瞬間
	}
}
