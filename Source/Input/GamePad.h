#pragma once

using GamePadButton = unsigned int;
using KeyBoardButton = unsigned int;

// ゲームパッド
class GamePad
{
public:
	static const GamePadButton BTN_UP				= (1 <<  0);
	static const GamePadButton BTN_RIGHT			= (1 <<  1);
	static const GamePadButton BTN_DOWN				= (1 <<  2);
	static const GamePadButton BTN_LEFT				= (1 <<  3);
	static const GamePadButton BTN_A				= (1 <<  4);
	static const GamePadButton BTN_B				= (1 <<  5);
	static const GamePadButton BTN_X				= (1 <<  6);
	static const GamePadButton BTN_Y				= (1 <<  7);
	static const GamePadButton BTN_START			= (1 <<  8);
	static const GamePadButton BTN_BACK				= (1 <<  9);
	static const GamePadButton BTN_LEFT_THUMB		= (1 << 10);
	static const GamePadButton BTN_RIGHT_THUMB		= (1 << 11);
	static const GamePadButton BTN_LEFT_SHOULDER	= (1 << 12);
	static const GamePadButton BTN_RIGHT_SHOULDER	= (1 << 13);
	static const GamePadButton BTN_LEFT_TRIGGER		= (1 << 14);
	static const GamePadButton BTN_RIGHT_TRIGGER	= (1 << 15);

	static const GamePadButton BTN_KEYBOARD_SPACE	= (1 << 16);
	static const GamePadButton BTN_KEYBOARD_F		= (1 << 17);
	static const GamePadButton BTN_KEYBOARD_R		= (1 << 18);
	static const GamePadButton BTN_KEYBOARD_P		= (1 << 19);
	static const GamePadButton BTN_KEYBOARD_TAB		= (1 << 20);
	static const GamePadButton BTN_KEYBOARD_SHIFT	= (1 << 21);
	static const GamePadButton BTN_KEYBOARD_E		= (1 << 22);
	static const GamePadButton BTN_KEYBOARD_ESC		= (1 << 23);
	static const GamePadButton BTN_KEYBOARD_C		= (1 << 24);
	static const GamePadButton BTN_KEYBOARD_V		= (1 << 25);
	static const GamePadButton BTN_KEYBOARD_CTLR	= (1 << 26);
	static const GamePadButton BTN_KEYBOARD_ALT		= (1 << 27);

public:
	GamePad() {}
	~GamePad() {}

	// 更新
	void Update();

	// スロット設定
	void SetSlot(int slot) { this->slot = slot; }

	// ボタン入力状態の取得
	GamePadButton GetButton() const { return buttonState[0]; }

	// ボタン押下状態の取得
	GamePadButton GetButtonDown() const { return buttonDown; }

	// ボタン押上状態の取得
	GamePadButton GetButtonUp() const { return buttonUp; }

	// 左スティックX軸入力状態の取得
	float GetAxisLX() const { return axisLx; }

	// 左スティックY軸入力状態の取得
	float GetAxisLY() const { return axisLy; }

	// 右スティックX軸入力状態の取得
	float GetAxisRX() const { return axisRx; }

	// 右スティックY軸入力状態の取得
	float GetAxisRY() const { return axisRy; }

	// 左トリガー入力状態の取得
	float GetTriggerL() const { return triggerL; }

	// 右トリガー入力状態の取得
	float GetTriggerR() const { return triggerR; }

	// 振動を設定するメソッド
	void SetVibration(float leftMotor, float rightMotor);

    // 振動を停止するメソッド
    void StopVibration() { SetVibration(0.0f, 0.0f); }

private:
	GamePadButton		buttonState[2] = { 0 };
	GamePadButton		buttonDown = 0;
	GamePadButton		buttonUp = 0;
	float				axisLx = 0.0f;
	float				axisLy = 0.0f;
	float				axisRx = 0.0f;
	float				axisRy = 0.0f;
	float				triggerL = 0.0f;
	float				triggerR = 0.0f;
	int					slot = 0;
};
