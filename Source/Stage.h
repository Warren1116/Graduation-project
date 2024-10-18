#pragma once

#include "Graphics/Shader.h"
#include "Collision.h"
#include "WayPoint.h"

// �X�e�[�W
class Stage
{
public:
	Stage() {}
	virtual ~Stage() {}

	// �X�V����
	virtual void Update(float elapsedTime) = 0;

	//// �`�揈��
	//virtual void Render(const RenderContext& rc, ModelShader* shader) = 0;

	// ���C�L���X�g
	virtual bool RayCast(const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		HitResult& hit) = 0;

	//// �E�F�C�|�C���g�̃C���f�b�N�X����|�W�V�������擾
	//virtual DirectX::XMFLOAT3 GetIndexWayPoint(int index) = 0;

	//// �w����W�����ԋ߂��E�F�C�|�C���g�̃C���f�b�N�X���擾
	//virtual int NearWayPointIndex(DirectX::XMFLOAT3 target) = 0;

	//// �ڑ���|�C���g��ݒ�
	//virtual void DestinationPointSet(int index) = 0;

	//virtual void DrawDebugPrimitive(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection) = 0;

	//virtual void DrawDebugGUI() = 0;

public:
	WayPoint* wayPoint[MAX_WAY_POINT];
};


