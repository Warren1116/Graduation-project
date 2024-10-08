#pragma once

#include "Graphics/Shader.h"
#include "Character.h"
#include "Audio/Audio.h"

// �O���錾
class ItemManager;

// �G�l�~�[
class Item : public Character
{
public:
	Item();
	~Item() override;

	// �X�V����
	virtual void Update(float elapsedTime) = 0;

	// �j��
	void Destroy();

	// �`�揈��
	//void Render(const RenderContext& rc, ModelShader* shader);

	// �f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();

	Model* GetModel() const { return model; }
	bool OkDestroy() const { return okDestroy; }

	bool CanPickup() const { return canPickup; }

protected:
	
	virtual void CollisionItemVsCrossHair() = 0;

	bool InRange(float range);

	// �s��X�V����
	void UpdateItemTransform()
	{
		// �X�P�[���s����쐬
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		DirectX::XMMATRIX CS = DirectX::XMMatrixScaling(cubeScale.x, cubeScale.y, cubeScale.z);
		// ��]�s����쐬
		DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
		DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
		DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);

		DirectX::XMMATRIX R = Y * X * Z;;

		// �ʒu�s����쐬
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
		// �R�̍s���g�ݍ��킹�A���[���h�s����쐬
		DirectX::XMMATRIX W = S * R * T;
		DirectX::XMMATRIX CW = CS * R * T;
		// �v�Z�������[���h�s������o��
		DirectX::XMStoreFloat4x4(&transform, W);
		DirectX::XMStoreFloat4x4(&cubeTransform, CW);
	}


protected:
	ItemManager* manager = nullptr;
	Model* model = nullptr;
	Model* cube = nullptr;
	DirectX::XMFLOAT3 cubeScale = { 1.0f, 1.5f, 0.5f };
	DirectX::XMFLOAT4X4 cubeTransform = transform;
	float itemRange = 5.0f;
	bool okDestroy = false;
	bool canPickup = false;
};

