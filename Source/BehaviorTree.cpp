//#include "BehaviorTree.h"
//#include "ActionBase.h"
//#include "NodeBase.h"
//#include "JudgmentBase.h"
//#include "PoisonZombie.h"
//#include "BehaviorData.h"
//
//
//// デストラクタ
//BehaviorTree::~BehaviorTree()
//{
//	NodeAllClear(root);
//}
//
//void BehaviorTree::AddNode(std::string parentName, std::string entryName, int priority, SelectRule selectRule, JudgmentBase* judgment, ActionBase* action)
//{
//	if (parentName != "")
//	{
//		NodeBase* parentNode = root->SearchNode(parentName);
//
//		if (parentNode != nullptr)
//		{
//			NodeBase* sibling = parentNode->GetLastChild();
//			NodeBase* addNode = new NodeBase(entryName, parentNode, sibling, priority, selectRule, judgment, action, parentNode->GetHirerchyNo() + 1);
//
//			parentNode->AddChild(addNode);
//		}
//	}
//	else {
//		if (root == nullptr)
//		{
//			root = new NodeBase(entryName, nullptr, nullptr, priority, selectRule, judgment, action, 1);
//		}
//	}
//}
//
//// 推論
//NodeBase* BehaviorTree::ActiveNodeInference(BehaviorData* data)
//{
//	// データをリセットして開始
//	data->Init();
//	return root->Inference(data);
//}
//
//// シーケンスノードからの推論開始
//NodeBase* BehaviorTree::SequenceBack(NodeBase* sequenceNode, BehaviorData* data)
//{
//	return sequenceNode->Inference(data);
//}
//
//// ノード実行
//NodeBase* BehaviorTree::Run(NodeBase* actionNode, BehaviorData* data,float elapsedTime)
//{
//	// ノード実行
//	ActionBase::State state = actionNode->Run(elapsedTime);
//
//	// 正常終了
//	if (state == ActionBase::State::Complete)
//	{
//		// シーケンスの途中かを判断
//		NodeBase* sequenceNode = data->PopSequenceNode();
//
//		// 途中じゃないなら終了
//		if (sequenceNode == nullptr)
//		{
//			return nullptr;
//		}
//		else {
//			// 途中ならそこから始める
//			return SequenceBack(sequenceNode, data);
//		}
//		// 失敗は終了
//	}
//	else if (state == ActionBase::State::Failed) {
//		return nullptr;
//	}
//
//	// 現状維持
//	return actionNode;
//}
//
//// 登録されたノードを全て削除する
//void BehaviorTree::NodeAllClear(NodeBase* delNode)
//{
//	size_t count = delNode->children.size();
//	if (count > 0)
//	{
//		for (NodeBase* node :delNode->children)
//		{
//			NodeAllClear(node);
//		}
//		delete delNode;
//	}
//	else
//	{
//		delete delNode;
//	}
//}
//
