#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"
#include "Effect.h"
#include "Object.h"
#include "SearchAlgorithm.h"

// �]���r
class TitleZombie : public Enemy
{
public:
	TitleZombie();
	~TitleZombie() override;

	// �X�V����
	void Update(float elapsedTime) override;

private:
	// �X�e�[�g�֑J��
	void TransitionTitleState();
	void UpdateTitleState(float elpsedTime);

	// ���S�X�e�[�g�֑J��
	void TransitionDeathState();

	// ���S�X�e�[�g�X�V����
	void UpdateDeathState(float elapsedTime);

private:
	// �X�e�[�g
	enum class State
	{
		Death,
		Title,
	};

	// �A�j���[�V����
	enum
	{
		Anim_Death,
		Anim_Attack1,
		Anim_RunFWD,
		Anim_IdleNormal,
	};

protected:
	// ���S�����Ƃ��ɌĂ΂��
	void OnDead() override;

private:
	State state = State::Title;
};