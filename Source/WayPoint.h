#pragma once
#include <DirectXMath.h>
#include "Edge.h"
#define ROW_COUNT 51
#define COLUM_COUNT 32
#define MAX_WAY_POINT ROW_COUNT * COLUM_COUNT

const int EdgeNo = 8;

// ウェイポイント用クラス(いわゆるノード)
class WayPoint
{
public:
	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	float range = 1.0f;
	bool obstacleFlag = false;
	WayPoint();

	~WayPoint();
	bool searchFg = false;
	Edge* edge[8]; // 各ノードをつなげるエッジ
	float costFromStart = 0.0f;
};