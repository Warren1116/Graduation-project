#pragma once

#include "Graphics/Shader.h"
#include "Effect.h"

// �O���錾
class ProjectileManager;

// �e��
class Projectile
{
public:
	Projectile(ProjectileManager* manager);
	//Projectile() {}
	virtual ~Projectile() {}

	// �X�V����
	virtual void Update(float elapsedTime) = 0;

	//// �`�揈��
	//virtual void Render(const RenderContext& rc, ModelShader* shader) = 0;

	void Destroy();

	// �f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();

	// �ʒu�擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// �����擾
	const DirectX::XMFLOAT3& GetDirection() const { return direction; }
	
	// �X�P�[���擾
	const DirectX::XMFLOAT3& GetScale()  const { return scale; }

	Model* GetModel() const { return model; }

	// ���a�擾
	float GetRadius() const { return radius; }

protected:
	// �s��X�V����
	void UpdateTransform();

protected:
	Model* model = nullptr;
	std::unique_ptr<Effect> destroyEffect = nullptr;
	DirectX::XMFLOAT3 position = { 0, 0, 0 };
	DirectX::XMFLOAT3 direction = { 0, 0, 1 };
	DirectX::XMFLOAT3 scale = { 1, 1, 1 };
	DirectX::XMFLOAT4X4 transform =
	{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	float radius = 0.166f;
	ProjectileManager* manager = nullptr;
};

