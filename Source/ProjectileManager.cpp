#include "ProjectileManager.h"
#include "Projectile.h"
#include "EnemyManager.h"
#include "Collision.h"
#include <ProjectileStraight.h>
ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
    Clear();
}

void ProjectileManager::Update(float elapsedTime)
{

    for (Projectile* projectile : projectiles)
    {
        projectile->Update(elapsedTime);


    }

    for (Projectile* projectile : removes)
    {
        std::vector<Projectile*>::iterator it = std::find(projectiles.begin(),
            projectiles.end(), projectile);
        if (it != projectiles.end())
        {
            projectiles.erase(it);
        }

        delete projectile;
    }

    removes.clear();
}

void ProjectileManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Projectile* projectile : projectiles)
    {
        projectile->Render(context, shader);
    }
}

void ProjectileManager::DrawDebugPrimitive()
{
    for (Projectile* projectile : projectiles)
    {
        projectile->DrawDebugPrimitive();
    }
}

void ProjectileManager::Register(Projectile* projectile)
{
    projectiles.emplace_back(projectile);
}

void ProjectileManager::Clear()
{
    for (Projectile* projectile : projectiles)
    {
        delete projectile;
    }
    projectiles.clear();
}


void ProjectileManager::Remove(Projectile* projectile)
{
    removes.insert(projectile);
}
