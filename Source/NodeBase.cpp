//#include "JudgmentBase.h"
//#include "NodeBase.h"
//#include "PoisonZombie.h"
//#include "BehaviorData.h"
//#include "ActionBase.h"
//
//
//
//// デストラクタ
//NodeBase::~NodeBase()
//{
//	delete judgment;
//	delete action;
//}
//// 子ノードゲッター
//NodeBase* NodeBase::GetChild(int index)
//{
//	if (children.size() <= index)
//	{
//		return nullptr;
//	}
//	return children.at(index);
//}
//
//// 子ノードゲッター(末尾)
//NodeBase* NodeBase::GetLastChild()
//{
//	if (children.size() == 0)
//	{
//		return nullptr;
//	}
//
//	return children.at(children.size() - 1);
//}
//
//// 子ノードゲッター(先頭)
//NodeBase* NodeBase::GetTopChild()
//{
//	if (children.size() == 0)
//	{
//		return nullptr;
//	}
//
//	return children.at(0);
//}
//
//
//// ノード検索
//NodeBase* NodeBase::SearchNode(std::string searchName)
//{
//	// 名前が一致
//	if (name == searchName)
//	{
//		return this;
//	}
//	else {
//		// 子ノードで検索
//		for (auto itr = children.begin(); itr != children.end(); itr++)
//		{
//			NodeBase* ret = (*itr)->SearchNode(searchName);
//
//			if (ret != nullptr)
//			{
//				return ret;
//			}
//		}
//	}
//
//	return nullptr;
//}
//
//// ノード推論
//NodeBase* NodeBase::Inference(BehaviorData* data)
//{
//	std::vector<NodeBase*> list;
//	NodeBase* result = nullptr;
//
//	// childrenの数だけループを行う。
//	for (int i = 0; i < children.size(); i++)
//	{
//		// children.at(i)->judgmentがnullptrでなければ
//		if (children.at(i)->judgment != nullptr)
//		{
//			// TODO 04_03 children.at(i)->judgment->Judgment()関数を実行し、tureであれば
//			// listにchildren.at(i)を追加していく
//			if (children.at(i)->judgment->Judgment())
//			{
//				list.emplace_back(children.at(i));
//			}
//		}
//		else {
//			// TODO 04_03 判定クラスがなければ無条件に追加
//			list.emplace_back(children.at(i));
//
//		}
//	}
//
//	// 選択ルールでノード決め
//	switch (selectRule)
//	{
//		// 優先順位
//	case BehaviorTree::SelectRule::Priority:
//		result = SelectPriority(&list);
//		break;
//		// ランダム
//	case BehaviorTree::SelectRule::Random:
//		result = SelectRandom(&list);
//		break;
//		// シーケンス
//	case BehaviorTree::SelectRule::Sequence:
//	case BehaviorTree::SelectRule::SequentialLooping:
//		result = SelectSequence(&list, data);
//		break;
//	}
//
//	if (result != nullptr)
//	{
//		// 行動があれば終了
//		if (result->HasAction() == true)
//		{
//			return result;
//		}
//		else {
//			// 決まったノードで推論開始
//			result = result->Inference(data);
//		}
//	}
//
//	return result;
//}
//
//// 優先順位でノード選択
//NodeBase* NodeBase::SelectPriority(std::vector<NodeBase*>* list)
//{
//	NodeBase* selectNode = nullptr;
//	int priority = INT_MAX;
//
//	// TODO 04_04 一番優先順位が高いノードを探してselectNodeに格納
//	for (auto itr = list->begin(); itr != list->end(); itr++)
//	{
//		if ((*itr)->GetPriority() < priority) {
//			priority = (*itr)->GetPriority();
//
//			selectNode = (*itr);
//		}
//	}
//
//	return selectNode;
//}
//
//
//
//// ランダムでノード選択
//NodeBase* NodeBase::SelectRandom(std::vector<NodeBase*>* list)
//{
//	int selectNo = 0;
//	// TODO 04_05 listのサイズで乱数を取得してselectNoに格納
//	selectNo = rand() % list->size();	
//	// listのselectNo番目の実態をリターン
//	return (*list).at(selectNo);
//}
//
//// シーケンス・シーケンシャルルーピングでノード選択
//NodeBase* NodeBase::SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data)
//{
//	int step = 0;
//
//	// 指定されている中間ノードのがシーケンスがどこまで実行されたか取得する
//	step = data->GetSequenceStep(name);
//
//	// 中間ノードに登録されているノード数以上の場合、
//	if (step >= children.size())
//	{
//		// TODO 04_06 ルールによって処理を切り替える
//		// ルールがBehaviorTree::SelectRule::SequentialLoopingのときは最初から実行するため、stepに0を代入
//		// ルールがBehaviorTree::SelectRule::Sequenceのときは次に実行できるノードがないため、nullptrをリターン
//		step = 0;
//		if (this->selectRule == BehaviorTree::SelectRule::Sequence)return nullptr;
//	}
//	// 実行可能リストに登録されているデータの数だけループを行う
//	for (auto itr = list->begin(); itr != list->end(); itr++)
//	{
//		// 子ノードが実行可能リストに含まれているか
//		if (children.at(step)->GetName() == (*itr)->GetName())
//		{
//			// TODO 04_06 現在の実行ノードの保存、次に実行するステップの保存を行った後、
//			// 現在のステップ番号のノードをリターンしなさい
//			// ①スタックにはdata->PushSequenceNode関数を使用する。保存するデータは実行中の中間ノード。
//			// ②また、次に実行する中間ノードとステップ数を保存する
//			// 　保存にはdata->SetSequenceStep関数を使用。
//			// 　保存データは中間ノードの名前と次のステップ数です(step + 1)
//			// ③ステップ番号目の子ノードを実行ノードとしてリターンする
//			data->PushSequenceNode(this->GetParent());
//			data->SetSequenceStep(this->GetName(), step + 1);
//
//			return list->at(step);
//		}
//	}
//	// 指定された中間ノードに実行可能ノードがないのでnullptrをリターンする
//	return nullptr;
//}
//
//// 判定
//bool NodeBase::Judgment()
//{
//	// TODO 04_07 judgmentがあるか判断。あればメンバ関数Judgment()実行した結果をリターン。
//	if (judgment != nullptr)
//	{
//		return judgment->Judgment();
//	}
//	return true;
//}
//
//// ノード実行
//ActionBase::State NodeBase::Run(float elapsedTime)
//{
//	// TODO 04_08 actionがあるか判断。あればメンバ関数Run()実行した結果をリターン。
//	if (action != nullptr)
//	{
//		return action->Run(elapsedTime);
//
//	}
//
//	return ActionBase::State::Failed;
//}
