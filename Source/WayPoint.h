#pragma once
#include <DirectXMath.h>
#include "Edge.h"
#define ROW_COUNT 51
#define COLUM_COUNT 32
#define MAX_WAY_POINT ROW_COUNT * COLUM_COUNT

const int EdgeNo = 8;

// �E�F�C�|�C���g�p�N���X(������m�[�h)
class WayPoint
{
public:
	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	float range = 1.0f;
	bool obstacleFlag = false;
	WayPoint();

	~WayPoint();
	bool searchFg = false;
	Edge* edge[8]; // �e�m�[�h���Ȃ���G�b�W
	float costFromStart = 0.0f;
};