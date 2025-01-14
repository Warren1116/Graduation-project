#pragma once

using KeyBoardButton = unsigned int;

// ゲームパッド
class KeyBoard
{
public:
	static const KeyBoardButton BTN_KEYBOARD_SPACE = (1 << 0);
	static const KeyBoardButton BTN_KEYBOARD_F = (1 << 1);
	static const KeyBoardButton BTN_KEYBOARD_R = (1 << 2);
	static const KeyBoardButton BTN_KEYBOARD_P = (1 << 3);
	static const KeyBoardButton BTN_KEYBOARD_TAB = (1 << 4);
	static const KeyBoardButton BTN_KEYBOARD_SHIFT = (1 << 5);
	static const KeyBoardButton BTN_KEYBOARD_E = (1 << 6);
	static const KeyBoardButton BTN_KEYBOARD_ESC = (1 << 7);
	static const KeyBoardButton BTN_KEYBOARD_C = (1 << 8);
	static const KeyBoardButton BTN_KEYBOARD_V = (1 << 9);
	static const KeyBoardButton BTN_KEYBOARD_W = (1 << 10);
	static const KeyBoardButton BTN_KEYBOARD_A = (1 << 11);
	static const KeyBoardButton BTN_KEYBOARD_S = (1 << 12);
	static const KeyBoardButton BTN_KEYBOARD_D = (1 << 13);


public:
	KeyBoard() {}
	~KeyBoard() {}

	// 更新
	void Update();

	// スロット設定
	void SetSlot(int slot) { this->slot = slot; }

	// ボタン入力状態の取得
	KeyBoardButton GetButton() const { return buttonState[0]; }

	// ボタン押下状態の取得
	KeyBoardButton GetButtonDown() const { return buttonDown; }

	// ボタン押上状態の取得
	KeyBoardButton GetButtonUp() const { return buttonUp; }

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



private:
	KeyBoardButton		buttonState[2] = { 0 };
	KeyBoardButton		buttonDown = 0;
	KeyBoardButton		buttonUp = 0;
	float				axisLx = 0.0f;
	float				axisLy = 0.0f;
	float				axisRx = 0.0f;
	float				axisRy = 0.0f;
	float				triggerL = 0.0f;
	float				triggerR = 0.0f;
	int					slot = 0;
};
