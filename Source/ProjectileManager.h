#pragma once
#include <vector>
#include "Projectile.h"
#include <set>;

class ProjectileManager
{
public:
    ProjectileManager();
    ~ProjectileManager();

    void Update(float elapsedTime);

    void Render(ID3D11DeviceContext* context, Shader* shader);

    void DrawDebugPrimitive();
    //’eŠÛ“o˜^
    void Register(Projectile* projectile);
    //’eŠÛ‘Síœ
    void Clear();
    //’eŠÛ”æ“¾
    int GetProjectileCount() const { return static_cast<int>(projectiles.size()); }
    //’eŠÛæ“¾
    Projectile* GetProjectile(int index) { return projectiles.at(index); }

    void Remove(Projectile* projectile);
private:
    std::vector<Projectile*> projectiles;
    std::set<Projectile*> removes;
};
