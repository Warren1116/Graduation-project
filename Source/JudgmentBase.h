#pragma once

class PoisonZombie;

// ���s����
class JudgmentBase
{
public:
	JudgmentBase(PoisonZombie* enemy) :owner(enemy) {}
	virtual bool Judgment() = 0;
protected:
	PoisonZombie* owner;
};