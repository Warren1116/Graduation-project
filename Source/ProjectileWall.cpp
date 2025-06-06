#pragma once
#include "ProjectileWall.h"
#include "SceneGame.h"
#include "ProjectileStraight.h"

ProjectileWall::ProjectileWall(BrokenProjectileManager* manager) : Projectile(manager)
{
    model = std::make_unique<Model>("Data/Model/SpiderWeb/WallWeb.mdl");
    scale.x = scale.y = scale.z = 0.1f;

}

ProjectileWall::~ProjectileWall()
{
}

void ProjectileWall::Update(float elapsedTime)
{
    //  壁にぶつかったあと3秒間存在する
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0)
    {
        SceneGame& sceneGame = SceneGame::Instance();
        sceneGame.UnregisterRenderModel(model.get());
        //  破棄
        Destroy();
    }
    UpdateTransform();
    model->UpdateTransform(transform);
}
