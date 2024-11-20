#pragma once

#include <DirectXMath.h>

// �X�t�B�A�L���X�g����
struct SphereCastResult
{
	DirectX::XMVECTOR	position = { 0, 0, 0 };	// ���C�ƃ|���S���̌�_
	DirectX::XMVECTOR	normal = { 0, 0, 0 };	// �Փ˂����|���S���̖@���x�N�g��
	float				distance = 0.0f; 		// ���C�̎n�_�����_�܂ł̋���
	DirectX::XMVECTOR	verts[3];
	int					materialIndex = -1; 	// �Փ˂����|���S���̃}�e���A���ԍ�
};

// �O���̓_�ɑ΂���O�p�`�����̍ŋߓ_���Z�o����
bool GetClosestPos_PointTriangle(
	const  DirectX::XMVECTOR& Point,
	const  DirectX::XMVECTOR TrianglePos[3],
	DirectX::XMVECTOR& nearPos,
	DirectX::XMVECTOR* nearTriPos1 = {},		// �O���̓_�ɑ΂��Ĉ�ԋ߂����_
	DirectX::XMVECTOR* nearTriPos2 = {});		// �O���̓_�ɑ΂���nearTriPos1�ƍ��킹�Ĉ�ԋ߂��ӂ��\�����钸�_

// ���CVs�O�p�`
bool IntersectRayVsTriangle(
	const DirectX::XMVECTOR& rayStart,
	const DirectX::XMVECTOR& rayDirection,		// �v���K��
	const float rayDist,
	const DirectX::XMVECTOR trianglePos[3],
	SphereCastResult* result = {});

// ��Vs�O�p�`
bool IntersectSphereVsTriangle(
	const DirectX::XMVECTOR& spherePos,
	const float radius,
	const DirectX::XMVECTOR trianglePos[3],
	SphereCastResult* result = {});

// ���CVs��
bool IntersectRayVsSphere(
	const DirectX::XMVECTOR& start,
	const DirectX::XMVECTOR& end,
	const DirectX::XMVECTOR& spherePos,
	const float radius,
	SphereCastResult* result = {});

// ���CVs�~��
bool IntersectRayVsCylinder(
	const DirectX::XMVECTOR& start,
	const DirectX::XMVECTOR& end,
	const DirectX::XMVECTOR& startCylinder,
	const DirectX::XMVECTOR& endCylinder,
	float radius,
	SphereCastResult* result = {});

// �X�t�B�A�L���X�gVs�O�p�` �ȈՔ�
bool IntersectSphereCastVsTriangleEST(
	const DirectX::XMVECTOR& start,			// �X�t�B�A�L���X�g�̃X�^�[�g�̈ʒu�̋��̒��S
	const DirectX::XMVECTOR& end,  			// �X�t�B�A�L���X�g�̃G���h�̈ʒu�̋��̒��S
	const float radius,
	const DirectX::XMVECTOR trianglePos[3],
	SphereCastResult* result = {},
	const float angle = -1.0f);				// �O�p�`�ƃX�t�B�A�L���X�g�̊Ԃ̊p�x���w��ȉ��̏ꍇ�A����������s��Ȃ��悤�ɂ���l�B�v���X�̒l�����W�A���Ŏw��B

// �X�t�B�A�L���X�gVs�O�p�` ���S��
bool IntersectSphereCastVsTriangle(
	const DirectX::XMVECTOR& start,			// �X�t�B�A�L���X�g�̃X�^�[�g�̈ʒu�̋��̒��S
	const DirectX::XMVECTOR& end,			// �X�t�B�A�L���X�g�̃G���h�̈ʒu�̋��̒��S
	const float radius,
	const DirectX::XMVECTOR trianglePos[3],
	SphereCastResult* result = {},
	const float angle = -1.0f);				// �O�p�`�ƃX�t�B�A�L���X�g�̊Ԃ̊p�x���w��ȉ��̏ꍇ�A����������s��Ȃ��悤�ɂ���l�B�v���X�̒l�����W�A���Ŏw��B

// �X�t�B�A�L���X�gVs��
bool IntersectSphereCastVsSphere(
	const DirectX::XMVECTOR& start,			// �X�t�B�A�L���X�g�̃X�^�[�g�̈ʒu�̋��̒��S
	const DirectX::XMVECTOR& end,  			// �X�t�B�A�L���X�g�̃G���h�̈ʒu�̋��̒��S
	const float sphereCastRadius,			// �X�t�B�A�L���X�g�̔��a
	const DirectX::XMVECTOR& spherePos,
	const float sphereRadius,				// ���̔��a
	SphereCastResult* result = {});

// �X�t�B�A�L���X�gVs�J�v�Z��
bool IntersectSphereCastVsCapsule(
	const DirectX::XMVECTOR& start,			// �X�t�B�A�L���X�g�̃X�^�[�g�̈ʒu�̋��̒��S
	const DirectX::XMVECTOR& end,  			// �X�t�B�A�L���X�g�̃G���h�̈ʒu�̋��̒��S
	const float sphereCastRadius,			// �X�t�B�A�L���X�g�̔��a
	const DirectX::XMVECTOR& startCapsule,
	const DirectX::XMVECTOR& endCapsule,
	const float capsuleRadius,				// �J�v�Z���̔��a
	SphereCastResult* result = {});

// �X�t�B�A�L���X�gVsAABB
bool IntersectSphereCastVsAABB(
	const DirectX::XMVECTOR& start,			// �X�t�B�A�L���X�g�̃X�^�[�g�̈ʒu�̋��̒��S
	const DirectX::XMVECTOR& end,  			// �X�t�B�A�L���X�g�̃G���h�̈ʒu�̋��̒��S
	const float sphereCastRadius,
	const DirectX::XMVECTOR& aabbPos,
	const DirectX::XMVECTOR& aabbRadii,
	SphereCastResult* result = {});
