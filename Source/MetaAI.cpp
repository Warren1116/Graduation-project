#include "MetaAI.h"

static Meta* instance = nullptr;
Meta& Meta::Instance()
{
	return *instance;
}
Meta::Meta(Player* pl, EnemyManager* eManager) : player(pl), enemyManager(eManager)
{
	instance = this;
}
void Meta::Update()
{
	// ここで自発的にメタAIが監視している処理があれば記載する
	// 今課題ではメタAIのイベントトリガーはメッセージを受信したときのため記述する処理は無し

}

// メッセージ受信したときの処理
bool Meta::OnMessage(const Telegram& telegram)
{
	std::vector<Enemy*> enmVec;
	enmVec.clear();
	int enemyCount = enemyManager->GetEnemyCount();
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MsgCallHelp://だれかがプレイヤーを発見したので他の敵を呼ぶ
		for (int i = 0; i < enemyCount; ++i)//全ての敵のリスト
		{
			int enemyId = enemyManager->GetEnemy(i)->GetId();
			// 送信者を除くエネミーにメッセージを送る
			if (telegram.sender != enemyId)
			{
				// 仲間を呼ぶメッセージの送信
				// 誰かがプレイヤーを発見したので送信者を除く敵にメッセージを送る
				// メタAIから各敵に送るメッセージも MsgCallHelp です。
				Meta::Instance().SendMessaging(
					static_cast<int>(Meta::Identity::Meta),
					enemyId,
					MESSAGE_TYPE::MsgCallHelp);
			}
		}
		return true;
		break;
	case MESSAGE_TYPE::MsgChangeAttackRight:
		//エネミーマネージャからAttackRange以内のエネミーだけの配列を作成し
		for (int i = 0; i < enemyCount; ++i)
		{
			Enemy* enemy = enemyManager->GetEnemy(i);
            if (!enemy->GetAlive()) continue;
			DirectX::XMFLOAT3 position = enemy->GetPosition();
			DirectX::XMFLOAT3 targetPosition = player->GetPosition();
			float vx = targetPosition.x - position.x;
			float vy = targetPosition.y - position.y;
			float vz = targetPosition.z - position.z;
			float dist = sqrtf(vx * vx + vy * vy + vz * vz);

			if (dist < enemy->GetAttackRange())
			{
				enmVec.push_back(enemy);
			}
		}
		if (enmVec.size() > 0)
		{
			// 攻撃権の付与
			// enmVecのサイズで乱数を取得し、取得した値をインデックス値として
			// enmVecに登録された敵の一人に攻撃権を与える
			// 攻撃権を与えるメッセージはMsgGiveAttackRightです。
			int random = rand() % enmVec.size();
			Meta::Instance().SendMessaging(
				static_cast<int>(Meta::Identity::Meta),
				enmVec.at(random)->GetId(),
				MESSAGE_TYPE::MsgGiveAttackRight);

		}
		return true;
		break;
	case MESSAGE_TYPE::MsgAskAttackRight: //敵から、最初の攻撃権を要求された
		int count = 0;//全ての敵について、攻撃権を持っていないか調べる。
		for (int i = 0; i < enemyCount; ++i)
		{
			Enemy* enemy = enemyManager->GetEnemy(i);
			if (enemy->GetAttackFlg()) count++;
		}
		//他に攻撃権持っている敵がいないなら、要求してきた敵に攻撃権を与えるメッセージを送る。
		//要求してきた敵のidはtelegramから取得出来ます。
		if (count == 0)
		{
			// 攻撃権の付与
			// 誰も攻撃権を持っていなければMetaAIから送信者に攻撃権付与のメッセージを送る
			// メッセージはMsgGiveAttackRight
			Meta::Instance().SendMessaging(
				static_cast<int>(Meta::Identity::Meta),
				telegram.sender,
				MESSAGE_TYPE::MsgGiveAttackRight);
		}
		return true;
		break;
	}
	return false;
}
void Meta::SendMessaging(int sender, int receiver, MESSAGE_TYPE msg)
{
	if (receiver == static_cast<int>(Meta::Identity::Meta))
	{// MetaAI宛の時
		//メッセージデータを作成
		Telegram telegram(sender, receiver, msg);
		// メッセージ送信
		OnMessage(telegram);
	}
	else
	{// エネミーが受信者のとき、どのエネミーに送信するか
		// 受信者のポインタを取得
		Enemy* receiveEnemy = enemyManager->GetEnemyFromId(receiver);
		//レシーバー居ないとき関数を終了する
		if (receiveEnemy == nullptr) return;
		//メッセージデータを作成
		Telegram telegram(sender, receiver, msg);
		//ディレイ無しメッセージ（即時配送メッセージ）
		receiveEnemy->OnMessage(telegram);
	}
}
