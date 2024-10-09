#pragma once
#include "ActionBase.h"
#include "PoisonZombie.h"
#include "Audio/Audio.h"

// �ʏ�U���ړ�
class NormalActionMove : public ActionBase
{
public:
	NormalActionMove(PoisonZombie* enemy) : ActionBase(enemy){}
	ActionBase::State Run(float elapsedTIme);
};

// �ʏ�U��
class NormalAction : public ActionBase
{
public:
	NormalAction(PoisonZombie* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// �e�U���ړ�
class GunActionMove : public ActionBase
{
public:
	GunActionMove(PoisonZombie* enemy) : ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
	std::unique_ptr<AudioSource> koe = nullptr;
	
};


// �W�����v�U���ړ�
class JumpActionMove : public ActionBase
{
public:
	JumpActionMove(PoisonZombie* enemy) : ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// �W�����v�U��
class JumpAction : public ActionBase
{
public:
	JumpAction(PoisonZombie* enemy) : ActionBase(enemy)	{}
	ActionBase::State Run(float elapsedTime);
};



//// �W�����v�ړ�
//class JumpPurAction : public ActionBase
//{
//public:
//	JumpPurAction(PoisonZombie* enemy) : ActionBase(enemy) {}
//	ActionBase::State Run(float elapsedTime);
//};

// �p�j�s��
//class WanderAction : public ActionBase
//{
//public:
//	WanderAction(PoisonZombie* enemy) :ActionBase(enemy) {}
//	ActionBase::State Run(float elapsedTime);
//};

// �ҋ@�s��
class IdleAction : public ActionBase
{
public:
	IdleAction(PoisonZombie* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

class DieAction : public ActionBase
{
public:
	DieAction(PoisonZombie* enemy) : ActionBase(enemy){}
	ActionBase::State Run(float elapsedTime);
}; 

class PursuitAction : public ActionBase
{
public:
	PursuitAction(PoisonZombie* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};