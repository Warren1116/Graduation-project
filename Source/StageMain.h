#pragma once

#include "Graphics/Model.h"
#include "Stage.h"
#include "WayPoint.h"

#define STAGE1	0
#define STAGE2	1
#define STAGE3	2

// �X�e�[�W
class StageMain : public Stage
{
public:
	StageMain();
	~StageMain() override;

	// �X�V����
	void Update(float elapsedTime) override;

	//// �`�揈��
	//void Render(const RenderContext& rc, ModelShader* shader) override;

	// ���C�L���X�g
	bool RayCast(const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		HitResult& hit) override;

	// �ʒu�ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
	DirectX::XMFLOAT3 GetPosition() const { return position; }

	DirectX::XMFLOAT3 GetIndexWayPoint(int index);

	int NearWayPointIndex(DirectX::XMFLOAT3 target);
	
	void DestinationPointSet(int index);

	void SetObstacle(int index);

	void DrawDebugPrimitive(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection) override;

	void DrawDebugGUI() override;

	// �C���X�^���X�擾
	static StageMain& Instance();

	static int GetStageNum();
	static void SetStageNum(int num);

	Model* GetModel() { return model; }

	//void SetEnemyObject(int index);

private:
	// �s��X�V
	void UpdateTransform()
	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
		DirectX::XMMATRIX W = S * R * T;
		DirectX::XMStoreFloat4x4(&transform, W);
	}

private:
	Model* model = nullptr;
	DirectX::XMFLOAT3 position = { 0, 0, 0 };
	DirectX::XMFLOAT3 angle = { 0, 0, 0 };
	DirectX::XMFLOAT3 scale = { 0.01f, 0.01f, 0.01f };
	DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
};