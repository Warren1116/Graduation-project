#pragma once

#include "Input/GamePad.h"
#include "Input/Mouse.h"
#include "Input\KeyBoard.h"

// インプット
class Input
{
public:
	Input(HWND hWnd);
	~Input() {}

public:
	// インスタンス取得
	static Input& Instance() { return *instance; }

	// 更新処理
	void Update();

	// ゲームパッド取得
	GamePad& GetGamePad() { return gamePad; }

	// マウス取得
	Mouse& GetMouse() { return mouse; }

    // キーボード取得
    KeyBoard& GetKeyBoard() { return keyBoard; }
private:
	static Input*		instance;
	GamePad				gamePad;
	Mouse				mouse;
    KeyBoard            keyBoard;
};
