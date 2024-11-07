#include "SwingWeb.h"
#include "SceneGame.h"
#include "ProjectileWall.h"
#include "Mathf.h"

SwingWeb* SwingWeb::instance = nullptr;

// コンストラクタ
SwingWeb::SwingWeb(ProjectileManager* manager) : Projectile(manager)
{
    instance = this;
    model = std::make_unique<Model>("Data/Model/SpiderWeb/SwingWeb.mdl");


    // 表示サイズ
    scale.x = scale.y = 0.0008f;
    scale.z = 0.5f;
    radius = 0.3f;

}

// デストラクタ
SwingWeb::~SwingWeb()
{

}

// 更新処理
void SwingWeb::Update(float elapsedTime)
{
    
    Player& player = Player::Instance();

    if (player.GetState() != Player::State::Swing)
    {
        SceneGame& sceneGame = SceneGame::Instance();
        if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
        {
            Destroy();
            sceneGame.UnregisterRenderModel(model.get());
        }
    }

    {
        //　糸は手から出るように、右手のNodeを探す
        Model::Node* RightHandPos = player.model->FindNode("mixamorig:RightHand");
        DirectX::XMFLOAT3 pos;
        pos.x = RightHandPos->worldTransform._41;
        pos.y = RightHandPos->worldTransform._42;
        pos.z = RightHandPos->worldTransform._43;

        DirectX::XMFLOAT3 swingPos = player.GetswingPoint();

        DirectX::XMFLOAT3 dir = swingPos - pos;
        DirectX::XMVECTOR dirVec = DirectX::XMLoadFloat3(&dir);
        dirVec = DirectX::XMVector3Normalize(dirVec);
        DirectX::XMStoreFloat3(&direction, dirVec);

        position = pos;
    }
   

    UpdateTransform();
    model->UpdateTransform(transform);

}

