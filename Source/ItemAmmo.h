#pragma once

#include "Item.h"
#include "ItemAmmo.h"

class ItemAmmo : public Item
{
public:
	ItemAmmo();
	~ItemAmmo() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();

private:
	

	void CollisionItemVsCrossHair() override;
};

