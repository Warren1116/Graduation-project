#include "Gun.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Player.h"
#include "ProjectileStraight.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "imgui.h"

static Gun* instance = nullptr;

Gun& Gun::Instance()
{
	return *instance;
}

Gun::Gun()
{
	instance = this;

	// ���f���ǂݍ���
	model = new Model("Data/Model/Gun/Gun.mdl");

	cameraController = new CameraController();

	reload = Audio::Instance().LoadAudioSource("Data/Audio/Reload.wav");
	slide = Audio::Instance().LoadAudioSource("Data/Audio/Slide.wav");
	fire = Audio::Instance().LoadAudioSource("Data/Audio/Fire.wav");
	hit = Audio::Instance().LoadAudioSource("Data/Audio/zombieDamage.wav");

	fireEffect = std::make_unique<Effect>("Data/Effect/hit.efk");

	position.x = 0;
	position.y = 0;
	position.z = 0;
}

Gun::~Gun()
{
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
}

void Gun::Update(float elapsedTime)
{
	Camera& camera = Camera::Instance();
	Player& player = Player::Instance();

	/*position = camera.GetEye();
	position.y -= 0.8f;*/

	/*DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&camera.GetFront()), 1.5f)));
	DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&camera.GetRight()), -0.014f)));*/

	// �m�[�h�擾
	Model::Node* node = player.model->FindNode("mixamorig:RightHandPinky3");

	if (node != nullptr)
	{
		// �m�[�h�ʒu�擾
		position.x = node->worldTransform._41;
		position.y = node->worldTransform._42;
		position.z = node->worldTransform._43;
	}

	DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&player.GetRight()), 1.0F)));
	DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&player.GetUp()), -10.0F)));
	DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&player.GetFront()), 6.0F)));

	angle.y = player.GetAngle().y;
	angle.y += DirectX::XMConvertToRadians(-90);

	DirectX::XMStoreFloat(&angle.z, DirectX::XMLoadFloat(&camera.GetFront().y));

	GamePad& gamePad = Input::Instance().GetGamePad();
	if ((gamePad.GetButtonDown() & GamePad::BTN_R) && haveAmmoNum < AMMO_MAX && player.GetState() != player.GetAttackState() && !isReload && player.GetHaveAmmoNum() > 0)
	{
		isReload = true;
		setReloadTime = true;
	}

	InputProjectile();

	// �e�ۍX�V����
	projectileManager.Update(elapsedTime);

	// �e�ۂƓG�̏Փˏ���
	CollisionProjectilesVsEnemies();

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

void Gun::InputProjectile()
{
	Mouse& mouse = Input::Instance().GetMouse();
	Camera& camera = Camera::Instance();
	Player& player = Player::Instance();

	// ���i�e�۔���
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		if (haveAmmoNum > 0)isFire = true;
		else if (!isReload && player.GetHaveAmmoNum() > 0)
		{
			isReload = true;
			setReloadTime = true;
		}
	}
	if (isFire)
	{

		// �O����
		DirectX::XMFLOAT3 dir;

		dir.x = transform._11;
		dir.y = transform._12;
		dir.z = transform._13;

		// ���ˈʒu
		Model::Node* node = model->FindNode("Cylinder.001");

		if (node != nullptr)
		{
			if (player.model->GetCurrentAnimationSeconds() < 0.251f && player.model->GetCurrentAnimationSeconds() > 0.249f)
			{
				// ����
				ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
				DirectX::XMFLOAT3 pos;
				pos.x = node->worldTransform._41;
				pos.y = node->worldTransform._42;
				pos.z = node->worldTransform._43;
				projectile->Launch(dir, pos);
				fire->Stop();
				fire->Play(false);
				fireEffect->Play(pos, 1.0f);
				isFire = false;
				haveAmmoNum--;
			}
		}
	}
}

void Gun::CollisionProjectilesVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	// ���ׂĂ̒e�ۂƂ��ׂĂ̓G�𑍓�����ŏՓˏ���
	int projectileCount = projectileManager.GetProjectileCount();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < projectileCount; ++i)
	{
		Projectile* projectile = projectileManager.GetProjectile(i);

		for (int j = 0; j < enemyCount; ++j)
		{
			Enemy* enemy = enemyManager.GetEnemy(j);

			// �Փˏ���
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectSphereVsCylinder(
				projectile->GetPosition(),
				projectile->GetRadius(),
				enemy->GetPosition(),
				enemy->GetRadius(),
				enemy->GetHeight(),
				outPosition))
			{
				// �_���[�W��^����
				if (enemy->ApplyDamage(1, 0.1f))
				{
					// ������΂�
					{
						DirectX::XMFLOAT3 impulse;
						const float power = 10.0f;
						const DirectX::XMFLOAT3& e = enemy->GetPosition();
						const DirectX::XMFLOAT3& p = projectile->GetPosition();
						float vx = e.x - p.x;
						float vz = e.z - p.z;
						float lengthXZ = sqrtf(vx * vx + vz * vz);
						vx /= lengthXZ;
						vz /= lengthXZ;

						impulse.x = vx * power;
						impulse.y = power * 0.5f;
						impulse.z = vz * power;

						//enemy->AddImpulse(impulse);
					}

					// �q�b�g�G�t�F�N�g�Đ�
					{
						DirectX::XMFLOAT3 enemyPos = enemy->GetPosition();
						enemyPos.y += enemy->GetHeight() * 0.5f;
					}

					hit->Stop();
					hit->Play(false);

					// �e�۔j��
					projectile->Destroy();
				}
			}
		}
	}
}

//void Gun::Render(const RenderContext& rc, ModelShader* shader)
//{
//	shader->Draw(rc, model);
//
//	// �e�ە`�揈��
//	projectileManager.Render(rc, shader);
//
//	// �e�ۃf�o�b�O�v���~�e�B�u�`��
//	projectileManager.DrawDebugPrimitive();
//}

void Gun::DrawDebugGUI()
{

}

// �s��X�V
void Gun::UpdateTransform()
{
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX W = S * R * T;
	DirectX::XMStoreFloat4x4(&transform, W);
}

void Gun::Reload()
{
	if (setReloadTime)
	{
		if (haveAmmoNum == 0) reloadTimer = 100;
		else reloadTimer = 25;

		setReloadTime = false;
	}
	if ((haveAmmoNum > 0 && reloadTimer == 25) || reloadTimer == 100) reload->Play(false);
	if (haveAmmoNum == 0 && reloadTimer == 25) slide->Play(false);

	if (reloadTimer < 0)
	{
		Player& player = Player::Instance();

		// �����[�h�������e��
		int insufficientNum = AMMO_MAX - haveAmmoNum;

		if (player.GetHaveAmmoNum() <= insufficientNum)
		{
			haveAmmoNum = player.GetHaveAmmoNum() + haveAmmoNum;
			player.SetHaveAmmoNum(player.GetHaveAmmoNum() - player.GetHaveAmmoNum());
		}
		else
		{
			haveAmmoNum = AMMO_MAX;
			player.SetHaveAmmoNum(player.GetHaveAmmoNum() - insufficientNum);
		}

		isReload = false;
	}

	reloadTimer--;
}
