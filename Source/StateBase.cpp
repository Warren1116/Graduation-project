#include "StateBase.h"

// HierarchicalState�N���X�̊֐���`���s��
// 2�w�ڃX�e�[�g�Z�b�g
void HierarchicalState::SetSubState(int newState)
{
	// TODO 03_02 �O���SetState�֐����Q�l�ɋL�q���Ȃ���
	subState = subStatePool.at(newState);

	subState->Enter();
}

// 2�w�ڂ̃X�e�[�g�؂�ւ�
void HierarchicalState::ChangeSubState(int newState)
{
	// �O���ChangeState�֐����Q�l�ɋL�q���Ȃ���
	if (newState >= 0 && newState < subStatePool.size())
	{
		if (subState != nullptr)
		{
			subState->Exit();
		}

		subState = subStatePool.at(newState);
		subState->Enter();
	}

}
// �T�u�X�e�[�g�o�^
void HierarchicalState::RegisterSubState(State* state)
{
	subStatePool.emplace_back(state);
}

// �T�u�X�e�[�g�̔ԍ��擾
int HierarchicalState::GetSubStateIndex()
{
	int i = 0;
	for (State* state : subStatePool)
	{
		if (subState == state)return i;
		++i;
	}
	return i;
}
