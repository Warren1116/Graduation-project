#include "ProjectileWall.h"
#include "SceneGame.h"
#include "ProjectileStraight.h"

ProjectileWall::ProjectileWall(ProjectileManager* manager) : Projectile(manager)
{
    model = std::make_unique<Model>("Data/Model/SpiderWeb/WallWeb.mdl");
    scale.x = scale.y = scale.z = 0.05f;
}

ProjectileWall::~ProjectileWall()
{
}

void ProjectileWall::Update(float elapsedTime)
{
    position = ProjectileStraight::Instance().GetPosition();
    direction = ProjectileStraight::Instance().GetDirection();


    UpdateTransform();
    model->UpdateTransform(transform);
}
