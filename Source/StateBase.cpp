#include "StateBase.h"

// HierarchicalStateクラスの関数定義を行う
// 2層目ステートセット
void HierarchicalState::SetSubState(int newState)
{
	subState = subStatePool.at(newState);

	subState->Enter();
}

// 2層目のステート切り替え
void HierarchicalState::ChangeSubState(int newState)
{
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
// サブステート登録
void HierarchicalState::RegisterSubState(State* state)
{
	subStatePool.emplace_back(state);
}

// サブステートの番号取得
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
