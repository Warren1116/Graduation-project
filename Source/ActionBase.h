#pragma once

class PoisonZombie;

// �s���������N���X
class ActionBase
{
public:
	ActionBase(PoisonZombie* enemy):owner(enemy){}
	// ���s���
	enum class State
	{
		Run,		// ���s��
		Failed,		// ���s���s
		Complete,	// ���s����
	};

	// ���s����(�������z�֐�)
	virtual ActionBase::State Run(float elapsedTime) = 0;
protected:
	PoisonZombie* owner;
	int step = 0;
};
