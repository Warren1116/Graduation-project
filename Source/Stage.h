#pragma once

#include "Graphics/Shader.h"
#include "Collision.h"

// ステージ
class Stage
{
public:
	Stage() {}
	virtual ~Stage() {}

	// 更新処理
	virtual void Update(float elapsedTime) = 0;

	// レイキャスト
	virtual bool RayCast(const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		HitResult& hit) = 0;

	virtual void DrawDebugPrimitive(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection) = 0;

};


