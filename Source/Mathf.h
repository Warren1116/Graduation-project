#pragma once

#include <DirectXMath.h>

inline DirectX::XMVECTOR operator+(DirectX::XMVECTOR V1, DirectX::XMVECTOR V2) {
	return DirectX::XMVectorAdd(V1, V2);
}

inline DirectX::XMVECTOR operator*(DirectX::XMVECTOR V1, DirectX::XMVECTOR V2) {
	return DirectX::XMVectorMultiply(V1, V2);
}

// 浮動小数算術
class Mathf
{
public:
	// 線形補完
	static float Lerp(float a, float b, float t);

	// 指定範囲をランダム値を計算する
	static float RandomRange(float min, float max);



};
