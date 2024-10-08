#include "StageTutorial.h"

StageTutorial::StageTutorial()
{
	// �X�e�[�W���f���ǂݍ���
	model = new Model("Data/Model/TutorialMap/TutorialMap.mdl");	
}

StageTutorial::~StageTutorial()
{
	// �X�e�[�W���f���̔j��
	delete model;
}

// �X�V����
void StageTutorial::Update(float elapsedTime)
{
	UpdateTransform();
	model->UpdateTransform(transform);
}

// ���C�L���X�g
bool StageTutorial::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model, hit);
}