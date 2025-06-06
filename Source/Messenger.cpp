#include "Messenger.h"

void Messenger::Clear()
{
	receivers.clear();
}

//　データ送信
void Messenger::SendData(const std::string& identifier, void* data)
{
	// 指定のキーの開始ノード終了ノードを取得
	auto itRange = receivers.equal_range(identifier);
	for (decltype(itRange.first) it = itRange.first; it != itRange.second; ++it)
	{
		it->second.func(data);
	}
}

// レシーバー追加
uint64_t Messenger::AddReceiver(const std::string& identifier, Receiver receiver)
{
	receivers.insert(std::make_pair(identifier, Data(incrementKey, receiver)));
	return incrementKey++;
}

// レシーバー削除
void Messenger::RemoveReceiver(uint64_t key)
{
	auto it = receivers.begin();
	auto itE = receivers.end();
	while (it != itE)
	{
		if (it->second.key == key)
		{
			receivers.erase(it);
			break;
		}
		it++;
	}
}

