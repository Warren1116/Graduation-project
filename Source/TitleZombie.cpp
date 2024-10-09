#include "TitleZombie.h"

TitleZombie::TitleZombie()
{
	model = new Model("Data/Model/Zombi/Zombi.mdl");

	// モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;

	radius = 0.35f;

	// 徘徊ステートへ遷移
	TransitionTitleState();
}

TitleZombie::~TitleZombie()
{
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}
}

void TitleZombie::Update(float elapsedTime)
{
	switch (state)
	{
	case State::Death:
		UpdateDeathState(elapsedTime);
		break;
	case State::Title:
		UpdateTitleState(elapsedTime);
	}

	// オブジェクト行列を更新
	UpdateTransform();

	// モデルアニメーション更新
	model->UpdateAnimation(elapsedTime);

	// モデル行列更新
	model->UpdateTransform(transform);
}

void TitleZombie::TransitionTitleState()
{
	state = State::Title;

	model->PlayAnimation(Anim_IdleNormal, true);
}

void TitleZombie::UpdateTitleState(float elpsedTime)
{
}

void TitleZombie::TransitionDeathState()
{
	state = State::Death;

	model->PlayAnimation(Anim_Death, false);
}

void TitleZombie::UpdateDeathState(float elapsedTime)
{
	if (!model->IsPlayAnimation())
	{
		//Destroy();
	}
}

// 死亡したときに呼ばれる
void TitleZombie::OnDead()
{
	TransitionDeathState();
}