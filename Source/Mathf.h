#pragma once

#include <DirectXMath.h>

//�@Vector���Z
// 2�̃x�N�g���̉��Z
inline DirectX::XMVECTOR operator+(DirectX::XMVECTOR V1, DirectX::XMVECTOR V2) {
    return DirectX::XMVectorAdd(V1, V2);
}

// 2�̃x�N�g���̌��Z
inline DirectX::XMVECTOR operator-(DirectX::XMVECTOR V1, DirectX::XMVECTOR V2) {
    return DirectX::XMVectorSubtract(V1, V2);
}

// 2��Float�̉��Z
inline DirectX::XMFLOAT3 operator+(DirectX::XMFLOAT3 V1, DirectX::XMFLOAT3 V2) {
    return { V1.x + V2.x, V1.y + V2.y, V1.z + V2.z };
}

// 2��FLoat�̌��Z
inline DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 V1, DirectX::XMFLOAT3 V2) {
    return { V1.x - V2.x, V1.y - V2.y, V1.z - V2.z };
}

// 2�̃x�N�g���̏�Z
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
