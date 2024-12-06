#pragma once

#include "Graphics/Model.h"
#include "Stage.h"
#include "WayPoint.h"


// ステージ
class StageMain : public Stage
{
private:
	struct CollisionMesh
	{
		struct Triangle
		{
			DirectX::XMFLOAT3	positions[3];
			DirectX::XMFLOAT3	normal;
		};
		std::vector<Triangle>	triangles;

		struct Area
		{
			DirectX::BoundingBox	boundingBox;
			std::vector<int>		triangleIndices;
		};
		std::vector<Area>		areas;
	};

public:
	StageMain();
	~StageMain() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		HitResult& hit) override;


	// 位置設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
	DirectX::XMFLOAT3 GetPosition() const { return position; }

	//DirectX::XMFLOAT3 GetIndexWayPoint(int index);

	//int NearWayPointIndex(DirectX::XMFLOAT3 target);
	
	//void DrawDebugGUI() override;

	void DrawDebugPrimitive(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection) override;

	// インスタンス取得
	static StageMain& Instance();

	static int GetStageNum();
	static void SetStageNum(int num);

	Model* GetModel() { return model.get(); }

	CollisionMesh& GetCollisionMesh() { return collisionMesh; }


private:
	// 行列更新
	void UpdateTransform()
	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
		DirectX::XMMATRIX W = S * R * T;
		DirectX::XMStoreFloat4x4(&transform, W);
	}

private:
	std::unique_ptr<Model> model;
	DirectX::XMFLOAT3 position = { 0, 0, 0 };
	DirectX::XMFLOAT3 angle = { 0, 0, 0 };
	DirectX::XMFLOAT3 scale = { 0.01f, 0.01f, 0.01f };
	DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	CollisionMesh	collisionMesh;

	DirectX::XMFLOAT3 volumeMin, volumeMax;


};