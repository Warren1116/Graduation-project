#pragma once

#include <DirectXMath.h>

//　Vector演算
// 2つのベクトルの加算
inline DirectX::XMVECTOR operator+(DirectX::XMVECTOR V1, DirectX::XMVECTOR V2) {
    return DirectX::XMVectorAdd(V1, V2);
}

// 2つのベクトルの減算
inline DirectX::XMVECTOR operator-(DirectX::XMVECTOR V1, DirectX::XMVECTOR V2) {
    return DirectX::XMVectorSubtract(V1, V2);
}

// 2つのFloatの加算
inline DirectX::XMFLOAT3 operator+(DirectX::XMFLOAT3 V1, DirectX::XMFLOAT3 V2) {
    return { V1.x + V2.x, V1.y + V2.y, V1.z + V2.z };
}

// 2つのFLoatの減算
inline DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 V1, DirectX::XMFLOAT3 V2) {
    return { V1.x - V2.x, V1.y - V2.y, V1.z - V2.z };
}

// 2つのベクトルの乗算
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
