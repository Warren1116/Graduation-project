#include "ProjectileStraight.h"
#include "StageManager.h"
#include "SceneGame.h"
#include "ProjectileWall.h"

ProjectileStraight* ProjectileStraight::instance = nullptr;

// �R���X�g���N�^
ProjectileStraight::ProjectileStraight(ProjectileManager* manager) : Projectile(manager)
{
    instance = this;
	model = std::make_unique<Model>("Data/Model/SpiderWeb/SpiderWeb.mdl");


	nohit = Audio::Instance().LoadAudioSource("Data/Audio/tyodan.wav");


	// �\���T�C�Y
	scale.x = scale.y = scale.z = 0.05f;
	radius = 0.3f;



}

// �f�X�g���N�^
ProjectileStraight::~ProjectileStraight()
{

}

// �X�V����
void ProjectileStraight::Update(float elapsedTime)
{
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)
    {
        SceneGame& sceneGame = SceneGame::Instance();
        if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
        {
            sceneGame.shadowmapRenderer->UnregisterRenderModel(model.get());
            sceneGame.sceneRenderer->UnregisterRenderModel(model.get());
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
        // ���˒e
        //DirectX::XMVECTOR ReflectVector = DirectX::XMVector3Reflect(DirectX::XMLoadFloat3(&direction), DirectX::XMLoadFloat3(&hit.normal));
        //DirectX::XMFLOAT3 reflectDirection;
        //DirectX::XMStoreFloat3(&reflectDirection, ReflectVector);
        //direction.x = reflectDirection.x;
        //direction.y = reflectDirection.y;
        //direction.z = reflectDirection.z;

        position.x = hit.position.x;
        position.y = hit.position.y;
        position.z = hit.position.z;

        Destroy();
        SceneGame& sceneGame = SceneGame::Instance();
        if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
        {
            sceneGame.shadowmapRenderer->UnregisterRenderModel(model.get());
            sceneGame.sceneRenderer->UnregisterRenderModel(model.get());
        }



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

// �`�揈��
//void ProjectileStraight::Render(const RenderContext& rc, ModelShader* shader)
//{
//	shader->Draw(rc, model);
//}

// ����
void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->direction = direction;
	this->position = position;
	
}

