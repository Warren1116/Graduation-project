#pragma once

#include "Graphics/Model.h"
#include "Stage.h"

// �`���[�g���A���X�e�[�W
class StageTutorial : public Stage
{
public:
	StageTutorial();
	~StageTutorial() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// ���C�L���X�g
	bool RayCast(const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		HitResult& hit) override;

	// �ʒu�ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
	DirectX::XMFLOAT3 GetPosition() const { return position; }

	//// �E�F�C�|�C���g�̃C���f�b�N�X����|�W�V�������擾
	//DirectX::XMFLOAT3 GetIndexWayPoint(int index) override { return DirectX::XMFLOAT3(0, 0, 0); }

	//// �w����W�����ԋ߂��E�F�C�|�C���g�̃C���f�b�N�X���擾
	//int NearWayPointIndex(DirectX::XMFLOAT3 target) override { return 0; }

	//// �ڑ���|�C���g��ݒ�
	//void DestinationPointSet(int index) override {}

	//void DrawDebugPrimitive(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection) override {}

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

	Model* model = nullptr;
	DirectX::XMFLOAT3 position = { 0, 0, 0 };
	DirectX::XMFLOAT3 angle = { 0, 0, 0 };
	DirectX::XMFLOAT3 scale = { 0.01f, 0.01f, 0.01f };
	DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
};