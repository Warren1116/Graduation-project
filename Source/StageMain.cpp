#include "StageMain.h"

//static Stage* instance = nullptr;
//Stage& Stage::Instance()
//{
//    return *instance;
//}

StageMain::StageMain()
{

    model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");

}
StageMain::~StageMain()
{
    delete model;
}

void StageMain::Update(float elapsedTime)
{

}
void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}
