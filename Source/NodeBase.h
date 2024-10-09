#pragma once

#include <vector>
#include <string>
#include "BehaviorTree.h"
#include "ActionBase.h"

class JudgmentBase;
class BehaviorData;

// メモリリーク調査用
#define debug_new new(_NORMAL_BLOCK,__FILE__,__LINE__)


// ノード
class NodeBase
{
public:
	// コンストラクタ
	NodeBase(std::string name,NodeBase* parent,NodeBase* sibling,int priority,
		BehaviorTree::SelectRule selectRule,JudgmentBase* judgment,ActionBase* action,int hierarchyNo) :
		name(name),parent(parent),sibling(sibling),priority(priority),
		selectRule(selectRule),judgment(judgment),action(action),hierarchyNo(hierarchyNo),
		children(NULL)
	{
	}
	// デストラクタ
	~NodeBase();
	// 名前ゲッター
	std::string GetName() { return name; }
	// 親ノードゲッター
	NodeBase* GetParent() { return parent; }
	// 子ノードゲッター
	NodeBase* GetChild(int index);
	// 子ノードゲッター(末尾)
	NodeBase* GetLastChild();
	// 子ノードゲッター(先頭)
	NodeBase* GetTopChild();
	// 兄弟ノードゲッター
	NodeBase* GetSibling() { return sibling; }
	// 階層番号ゲッター
	int GetHirerchyNo() { return hierarchyNo; }
	// 優先順位ゲッター
	int GetPriority() { return priority; }
	// 親ノードセッター
	void SetParent(NodeBase* parent) { this->parent = parent; }
	// 子ノード追加
	void AddChild(NodeBase* child) { children.push_back(child); }
	// 兄弟ノードセッター
	void SetSibling(NodeBase* sibling) {this->sibling = sibling;}
	// 行動データを持っているか
	bool HasAction() { return action != nullptr ? true : false; }
	// 実行可否判定
	bool Judgment();
	// 優先順位選択
	NodeBase* SelectPriority(std::vector<NodeBase*>* list);
	// ランダム選択
	NodeBase* SelectRandom(std::vector<NodeBase*>* list);
	// シーケンス選択
	NodeBase* SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data);
	// ノード検索
	NodeBase* SearchNode(std::string searchName);
	// ノード推論
	NodeBase* Inference(BehaviorData* data);
	// 実行
	ActionBase::State Run(float elapsedTime);
	std::vector<NodeBase*>		children;		// 子ノード
protected:
	std::string					name;			// 名前
	BehaviorTree::SelectRule	selectRule;		// 選択ルール
	JudgmentBase*				judgment;	// 判定クラス
	ActionBase*					action;			// 実行クラス
	unsigned int				priority;		// 優先順位
	NodeBase*					parent;			// 親ノード
	NodeBase*					sibling;		// 兄弟ノード
	int							hierarchyNo;	// 階層番号
};