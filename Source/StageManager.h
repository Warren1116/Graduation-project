#pragma once

#include <vector>
#include "Stage.h"

class StageManager
{
private:
	StageManager() {}
	~StageManager() {}

public:
	// �B��̃C���X�^���X�擾
	static StageManager& Instance()
	{
		static StageManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �X�e�[�W�o�^
	void Register(Stage* stage);

	// �X�e�[�W�S�폜
	void Clear();

	// ���C�L���X�g
	bool RayCast(const DirectX::XMFLOAT3& start, 
		const DirectX::XMFLOAT3& end, 
		HitResult& hit);

	// �X�e�[�W�擾
	Stage* GetStage(int index) { return stages.at(index); }

	void DrawDebugPrimitive(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

private:
	std::vector<Stage*> stages;
};

