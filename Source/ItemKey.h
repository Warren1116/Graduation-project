#pragma once

#include "Graphics/Shader.h"
#include "Character.h"
#include "Audio/Audio.h"
#include "Item.h"

// �G�l�~�[
class ItemKey : public Item
{
public:
	ItemKey();
	~ItemKey() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	//void Render(const RenderContext& rc, ModelShader* shader);

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive() override;

	Model* GetModel() const { return model; }
	bool OkDestroy() const { return okDestroy; }

	bool CanPickup() const { return canPickup; }

protected:

	void CollisionItemVsCrossHair() override;

protected:
	
};

