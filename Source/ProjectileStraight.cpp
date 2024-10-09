#include "ProjectileStraight.h"
#include "StageManager.h"
#include "SceneGame.h"


// コンストラクタ
ProjectileStraight::ProjectileStraight(ProjectileManager* manager) : Projectile(manager)
{
	model = new Model("Data/Model/SpiderWeb/SpiderWeb.mdl");

	nohit = Audio::Instance().LoadAudioSource("Data/Audio/tyodan.wav");


	// 表示サイズ
	scale.x = scale.y = scale.z = 0.05f;
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
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)
    {
        SceneGame& sceneGame = SceneGame::Instance();

        if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
        {
            sceneGame.shadowmapRenderer->UnregisterRenderModel(model);
            sceneGame.sceneRenderer->UnregisterRenderModel(model);
        }
        Destroy();
    }


    float speed = this->speed * elapsedTime;

    float newX = position.x + direction.x * speed;
    float newY = position.y + direction.y * speed;
    float newZ = position.z + direction.z * speed;

    DirectX::XMFLOAT3 start = { position.x, position.y, position.z };
    DirectX::XMFLOAT3 end = { newX, newY, newZ };

    HitResult hit;
    if (StageManager::Instance().RayCast(start, end, hit))
    {
        DirectX::XMVECTOR ReflectVector = DirectX::XMVector3Reflect(DirectX::XMLoadFloat3(&direction), DirectX::XMLoadFloat3(&hit.normal));
        DirectX::XMFLOAT3 reflectDirection;
        DirectX::XMStoreFloat3(&reflectDirection, ReflectVector);

        direction.x = reflectDirection.x;
        direction.y = reflectDirection.y;
        direction.z = reflectDirection.z;

        position.x = hit.position.x;
        position.y = hit.position.y;
        position.z = hit.position.z;
    }
    else
    {
        position.x = newX;
        position.y = newY;
        position.z = newZ;
    }


    UpdateTransform();
    model->UpdateTransform(transform);

}

// 描画処理
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
