#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"
#include "Effect.h"
#include "Object.h"
#include "SearchAlgorithm.h"

// ゾンビ
class TitleZombie : public Enemy
{
public:
	TitleZombie();
	~TitleZombie() override;

	// 更新処理
	void Update(float elapsedTime) override;

private:
	// ステートへ遷移
	void TransitionTitleState();
	void UpdateTitleState(float elpsedTime);

	// 死亡ステートへ遷移
	void TransitionDeathState();

	// 死亡ステート更新処理
	void UpdateDeathState(float elapsedTime);

private:
	// ステート
	enum class State
	{
		Death,
		Title,
	};

	// アニメーション
	enum
	{
		Anim_Death,
		Anim_Attack1,
		Anim_RunFWD,
		Anim_IdleNormal,
	};

protected:
	// 死亡したときに呼ばれる
	void OnDead() override;

private:
	State state = State::Title;
};