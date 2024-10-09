#pragma once
#include <vector>
#include "StateBase.h"

class StateMachine
{
public:
	// コンストラクタ
	StateMachine() {}
	// デストラクタ
	~StateMachine();
	// 更新処理
	void Update(float elapsedTime);
	// ステートセット
	void SetState(int setstate);
	// ステート変更
	void ChangeState(int newState);
	// ステート登録
	void RegisterState(State* state);
	// 現在のステート番号取得
	int GetStateIndex();
private:
	// 現在のステート
	State* currentState = nullptr;
	// 各ステートを保持する配列
	std::vector<State*> statePool;
};