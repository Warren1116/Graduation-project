#pragma once
#include "ActionBase.h"
#include "PoisonZombie.h"
#include "Audio/Audio.h"

// ’ÊíUŒ‚ˆÚ“®
class NormalActionMove : public ActionBase
{
public:
	NormalActionMove(PoisonZombie* enemy) : ActionBase(enemy){}
	ActionBase::State Run(float elapsedTIme);
};

// ’ÊíUŒ‚
class NormalAction : public ActionBase
{
public:
	NormalAction(PoisonZombie* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// eUŒ‚ˆÚ“®
class GunActionMove : public ActionBase
{
public:
	GunActionMove(PoisonZombie* enemy) : ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
	std::unique_ptr<AudioSource> koe = nullptr;
	
};


// ƒWƒƒƒ“ƒvUŒ‚ˆÚ“®
class JumpActionMove : public ActionBase
{
public:
	JumpActionMove(PoisonZombie* enemy) : ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// ƒWƒƒƒ“ƒvUŒ‚
class JumpAction : public ActionBase
{
public:
	JumpAction(PoisonZombie* enemy) : ActionBase(enemy)	{}
	ActionBase::State Run(float elapsedTime);
};



//// ƒWƒƒƒ“ƒvˆÚ“®
//class JumpPurAction : public ActionBase
//{
//public:
//	JumpPurAction(PoisonZombie* enemy) : ActionBase(enemy) {}
//	ActionBase::State Run(float elapsedTime);
//};

// œpœjs“®
//class WanderAction : public ActionBase
//{
//public:
//	WanderAction(PoisonZombie* enemy) :ActionBase(enemy) {}
//	ActionBase::State Run(float elapsedTime);
//};

// ‘Ò‹@s“®
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