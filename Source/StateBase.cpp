#include "StateBase.h"

// HierarchicalStateクラスの関数定義を行う
// 2層目ステートセット
void EnemyHierarchicalState::SetSubState(int newState)
{
	subState = subStatePool.at(newState);

	subState->Enter();
}

// 2層目のステート切り替え
void EnemyHierarchicalState::ChangeSubState(int newState)
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
void EnemyHierarchicalState::RegisterSubState(EnemyState* state)
{
	subStatePool.emplace_back(state);
}

// サブステートの番号取得
int EnemyHierarchicalState::GetSubStateIndex()
{
	int i = 0;
	for (EnemyState* state : subStatePool)
	{
		if (subState == state)return i;
		++i;
	}
	return i;
}
