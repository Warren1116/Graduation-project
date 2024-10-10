#pragma once

// 送信するメッセージの種類
enum class MESSAGE_TYPE : int
{
	MsgCallHelp,				// 仲間を呼ぶ(Enemy、メタAIの両方で使用)
	MsgChangeAttackRight,		// 攻撃権の移譲(EnemyからメタAIへのメッセージ用)
	MsgGiveAttackRight,			// 攻撃権を与える（メタAIからEnemyへのメッセージ用）
	MsgAskAttackRight,			// 攻撃権を要求(EnemyからメタAIへのメッセージ用)
};

class Telegram
{
public:
	int sender;			// 送り手のID
	int receiver;		// 受けてのID
	MESSAGE_TYPE msg;	// メッセージNO

	//コンストラクタ
	Telegram(int sender, int receiver, MESSAGE_TYPE msg) :sender(sender), receiver(receiver), msg(msg)
	{}
};
