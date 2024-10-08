#include "StageTutorial.h"

StageTutorial::StageTutorial()
{
	// ステージモデル読み込み
	model = new Model("Data/Model/TutorialMap/TutorialMap.mdl");	
}

StageTutorial::~StageTutorial()
{
	// ステージモデルの破棄
	delete model;
}

// 更新処理
void StageTutorial::Update(float elapsedTime)
{
	UpdateTransform();
	model->UpdateTransform(transform);
}

// レイキャスト
bool StageTutorial::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model, hit);
}