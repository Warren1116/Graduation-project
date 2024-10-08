#include "ProjectileStraight.h"
#include "StageManager.h"

// コンストラクタ
ProjectileStraight::ProjectileStraight(ProjectileManager* manager)
	:Projectile(manager)
{
	// model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
	model = new Model("Data/Model/Sword/Sword.mdl");

	nohit = Audio::Instance().LoadAudioSource("Data/Audio/tyodan.wav");


	// 表示サイズ
	scale.x = scale.y = scale.z = 2.0f;
	radius = 0.3f;
}

// デストラクタ
ProjectileStraight::~ProjectileStraight()
{
	delete model;
}

// 更新処理
void ProjectileStraight::Update(float elapsedTime)
{
	// 移動
	float speed = this->speed * elapsedTime;
	position.x += speed * direction.x;
	position.y += speed * direction.y;
	position.z += speed * direction.z;

	// オブジェクト行列を更新
	UpdateTransform();

	// 寿命処理
	lifeTimer -= elapsedTime;
	if (lifeTimer <= 0.0f)
	{
		// 自分を削除
		Destroy();
	}

	if (one)
	{
		
		destroyEffect->Play(position);
		Destroy();
	}

	HitResult hit;
	float mx = (speed * direction.x * 100) * elapsedTime;
	float my = (speed * direction.y * 100) * elapsedTime;
	float mz = (speed * direction.z * 100) * elapsedTime;

	// レイの開始位置と終点位置
	DirectX::XMFLOAT3 start = { position.x, position.y, position.z };
	DirectX::XMFLOAT3 end = { position.x + mx, position.y + my, position.z + mz };
	if (StageManager::Instance().RayCast(start, end, hit))
	{
		one = true;
		nohit->Stop();
		nohit->Play(false);
	}

	// モデル行列更新
	model->UpdateTransform(transform);
}

//// 描画処理
//void ProjectileStraight::Render(const RenderContext& rc, ModelShader* shader)
//{
//	shader->Draw(rc, model);
//}

// 発射
void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->direction = direction;
	this->position = position;
	
}
