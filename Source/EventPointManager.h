#pragma once

#include <string>
#include <vector>

#include "EventData.h"

// イベントポイント管理者
class EventPointManager
{
private:
	EventPointManager() {}
	~EventPointManager() {}

public:
	// 唯一のインスタンス取得
	static EventPointManager& Instance()
	{
		static EventPointManager instance;
		return instance;
	}

	// 読み込み
	void	LoadPoints(const char* filepath);

	// クリア
	void	Clear();

	// ポイント範囲チェック
	void	CheckPoint(const DirectX::XMFLOAT3& position, float radius);

	// ポイント範囲描画
	void	DrawDebugPrimitive();

	inline	std::vector<EventData>& GetPoints() { return points; }

private:
	std::vector<EventData> points;
};
