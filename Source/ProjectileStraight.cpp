#include "ProjectileStraight.h"
#include "StageManager.h"

// �R���X�g���N�^
ProjectileStraight::ProjectileStraight(ProjectileManager* manager)
	:Projectile(manager)
{
	// model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
	model = new Model("Data/Model/Sword/Sword.mdl");

	nohit = Audio::Instance().LoadAudioSource("Data/Audio/tyodan.wav");


	// �\���T�C�Y
	scale.x = scale.y = scale.z = 2.0f;
	radius = 0.3f;
}

// �f�X�g���N�^
ProjectileStraight::~ProjectileStraight()
{
	delete model;
}

// �X�V����
void ProjectileStraight::Update(float elapsedTime)
{
	// �ړ�
	float speed = this->speed * elapsedTime;
	position.x += speed * direction.x;
	position.y += speed * direction.y;
	position.z += speed * direction.z;

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ��������
	lifeTimer -= elapsedTime;
	if (lifeTimer <= 0.0f)
	{
		// �������폜
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

	// ���C�̊J�n�ʒu�ƏI�_�ʒu
	DirectX::XMFLOAT3 start = { position.x, position.y, position.z };
	DirectX::XMFLOAT3 end = { position.x + mx, position.y + my, position.z + mz };
	if (StageManager::Instance().RayCast(start, end, hit))
	{
		one = true;
		nohit->Stop();
		nohit->Play(false);
	}

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

//// �`�揈��
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
