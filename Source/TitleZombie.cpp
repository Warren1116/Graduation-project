#include "TitleZombie.h"

TitleZombie::TitleZombie()
{
	model = new Model("Data/Model/Zombi/Zombi.mdl");

	// ���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;

	radius = 0.35f;

	// �p�j�X�e�[�g�֑J��
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

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���A�j���[�V�����X�V
	model->UpdateAnimation(elapsedTime);

	// ���f���s��X�V
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

// ���S�����Ƃ��ɌĂ΂��
void TitleZombie::OnDead()
{
	TransitionDeathState();
}