#pragma once

#include <vector>
#include "Stage.h"

class StageManager
{
private:
	StageManager() {}
	~StageManager() {}

public:
	// 唯一のインスタンス取得
	static StageManager& Instance()
	{
		static StageManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsedTime);

	// ステージ登録
	void Register(Stage* stage);

	// ステージ全削除
	void Clear();

	// レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, 
		const DirectX::XMFLOAT3& end, 
		HitResult& hit);

	// ステージ取得
	Stage* GetStage(int index) { return stages.at(index); }

	void DrawDebugPrimitive(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

private:
	std::vector<Stage*> stages;
};

