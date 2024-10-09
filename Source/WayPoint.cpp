#include "WayPoint.h"

WayPoint::WayPoint()
{
	position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < EdgeNo; ++i)
	{
		edge[i] = nullptr;
	}
}
WayPoint::~WayPoint()
{
	for (int i = 0; i < EdgeNo; ++i)
	{
		if (edge[i] != nullptr)
			delete edge[i];
	}
}