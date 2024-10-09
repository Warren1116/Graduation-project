#pragma once
#include "JudgmentBase.h"
#include "PoisonZombie.h"


// BattleNode�ɑJ�ڂł��邩����
class BattleJudgment : public JudgmentBase
{
public:
	BattleJudgment(PoisonZombie* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// HandAttackNode�ɑJ�ڂł��邩����
class HandAttackMoveJudgment : public JudgmentBase
{
public:
	HandAttackMoveJudgment(PoisonZombie* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// HandAttackNode�ɑJ�ڂł��邩����
class HandAttackJudgment : public JudgmentBase
{
public:
	HandAttackJudgment(PoisonZombie* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// AttackNode�ɑJ�ڂł��邩����
class WnderJudgment : public JudgmentBase
{
public:
	WnderJudgment(PoisonZombie* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

//// JumpNode�ɑJ�ڂł��邩����
class JumpAttackMoveJudgment : public JudgmentBase
{
public:
	JumpAttackMoveJudgment(PoisonZombie* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

class JumpAttackJudgment : public JudgmentBase
{
public:
	JumpAttackJudgment(PoisonZombie* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

class DieJudgment : public JudgmentBase
{
public:
	DieJudgment(PoisonZombie* enemy) : JudgmentBase(enemy) {};
	bool Judgment();
};
