#pragma once

#include "Graphics/Shader.h"
#include "Collision.h"
#include "WayPoint.h"

// ステージ
class Stage
{
public:
	Stage() {}
	virtual ~Stage() {}

	// 更新処理
	virtual void Update(float elapsedTime) = 0;

	//// 描画処理
	//virtual void Render(const RenderContext& rc, ModelShader* shader) = 0;

	// レイキャスト
	virtual bool RayCast(const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		HitResult& hit) = 0;

	//// ウェイポイントのインデックスからポジションを取得
	//virtual DirectX::XMFLOAT3 GetIndexWayPoint(int index) = 0;

	//// 指定座標から一番近いウェイポイントのインデックスを取得
	//virtual int NearWayPointIndex(DirectX::XMFLOAT3 target) = 0;

	//// 接続先ポイントを設定
	//virtual void DestinationPointSet(int index) = 0;

	//virtual void DrawDebugPrimitive(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection) = 0;

	//virtual void DrawDebugGUI() = 0;

public:
	WayPoint* wayPoint[MAX_WAY_POINT];
};


