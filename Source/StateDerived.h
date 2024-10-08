#pragma once
#include "PoisonZombie.h"


// 徘徊ステートオブジェクト
class WanderState : public State
{
public:
	// コンストラクタ
	WanderState(PoisonZombie* enemy) :State(enemy) {};
	// デストラクタ
	~WanderState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 待機ステートオブジェクト
class IdleState : public State
{
public:
	// コンストラクタ
	IdleState(PoisonZombie* enemy) :State(enemy) {};
	// デストラクタ
	~IdleState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 追跡ステートオブジェクト
class PursuitState : public State
{
public:
	// コンストラクタ
	PursuitState(PoisonZombie* enemy) :State(enemy) {};
	// デストラクタ
	~PursuitState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 攻撃待機ステートオブジェクト
class AttackIdleState : public State
{
public:
	// コンストラクタ
	AttackIdleState(PoisonZombie* enemy) :State(enemy) {};
	// デストラクタ
	~AttackIdleState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};


// 毒攻撃ステート
class GunState : public State
{
public:
	// コンストラクタ
	GunState(PoisonZombie* enemy) : State(enemy) {};
	// デストラクタ
	~GunState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するときのメソッド
	void Execute(float elapsedTime) override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 手刀攻撃ステート
class HandState : public State
{
public:
	// コンストラクタ
	HandState(PoisonZombie* enemy) : State(enemy) {};
	// デストラクタ
	~HandState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するときのメソッド
	void Execute(float elapsedTime) override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// ジャンプ攻撃ステート
class JumpState : public State
{
public:
	// コンストラクタ
	JumpState(PoisonZombie* enemy) : State(enemy) {};
	// デストラクタ
	~JumpState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するときのメソッド
	void Execute(float elapsedTime) override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};