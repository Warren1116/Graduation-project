#pragma once

#include <DirectXMath.h>

inline DirectX::XMVECTOR operator+(DirectX::XMVECTOR V1, DirectX::XMVECTOR V2) {
	return DirectX::XMVectorAdd(V1, V2);
}

inline DirectX::XMVECTOR operator*(DirectX::XMVECTOR V1, DirectX::XMVECTOR V2) {
	return DirectX::XMVectorMultiply(V1, V2);
}

// ���������Z�p
class Mathf
{
public:
	// ���`�⊮
	static float Lerp(float a, float b, float t);

	// �w��͈͂������_���l���v�Z����
	static float RandomRange(float min, float max);



};
