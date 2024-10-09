#pragma once
#include "JudgmentBase.h"
#include "PoisonZombie.h"


// BattleNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class BattleJudgment : public JudgmentBase
{
public:
	BattleJudgment(PoisonZombie* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();
};

// HandAttackNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class HandAttackMoveJudgment : public JudgmentBase
{
public:
	HandAttackMoveJudgment(PoisonZombie* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();
};

// HandAttackNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class HandAttackJudgment : public JudgmentBase
{
public:
	HandAttackJudgment(PoisonZombie* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();
};

// AttackNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class WnderJudgment : public JudgmentBase
{
public:
	WnderJudgment(PoisonZombie* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();
};

//// JumpNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class JumpAttackMoveJudgment : public JudgmentBase
{
public:
	JumpAttackMoveJudgment(PoisonZombie* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();
};

class JumpAttackJudgment : public JudgmentBase
{
public:
	JumpAttackJudgment(PoisonZombie* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();
};

class DieJudgment : public JudgmentBase
{
public:
	DieJudgment(PoisonZombie* enemy) : JudgmentBase(enemy) {};
	bool Judgment();
};
