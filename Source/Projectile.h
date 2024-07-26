#pragma once
#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"
#include "Collision.h"

class ProjectileManager;

class Projectile
{
public:
    //Projectile() {}
    Projectile(ProjectileManager* manager);
    virtual ~Projectile() {}
    //更新処理
    virtual void Update(float elapsedTime) = 0;
    //描画処理
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;
    //デバッグプリミティブ描画
    virtual void DrawDebugPrimitive();
    //位置取得
    const DirectX::XMFLOAT3& GetPosition() const { return position; }
    //方向取得
    const DirectX::XMFLOAT3& GetDirection() const { return direction; }
    //スケール取得
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    float GetRadius() const { return radius; }
    
    void Destroy();

protected:
    //行列更新処理
    void UpdateTransform();

protected:
    DirectX::XMFLOAT3 position = { 0,0,0 };
    float radius = { 0.5f };
    DirectX::XMFLOAT3 direction = { 0,0,1 };
    DirectX::XMFLOAT3 scale = { 1,1,1 };
    DirectX::XMFLOAT4X4 transform = { 
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1 };
    ProjectileManager* manager = nullptr;
};