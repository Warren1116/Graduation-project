#include "SwingWeb.h"
#include "SceneGame.h"
#include "ProjectileWall.h"
#include "Mathf.h"

SwingWeb* SwingWeb::instance = nullptr;
SwingWeb* SwingWeb::instanceLeft = nullptr;
SwingWeb* SwingWeb::instanceRight = nullptr;

// コンストラクタ
SwingWeb::SwingWeb(ProjectileManager* manager, bool leftHand) : Projectile(manager), isLeftHand(leftHand)
{
    //instance = this;

    if (isLeftHand)
    {
        instanceLeft = this;
    }
    else
    {
        instanceRight = this;
    }


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
    if (!player.GetIsUseGrab())
    {
        if (player.GetState() != Player::State::Swing)
        {
            SceneGame& sceneGame = SceneGame::Instance();
            if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            {
                Destroy();
                sceneGame.UnregisterRenderModel(model.get());
            }
        }

        DirectX::XMFLOAT3 handPos;
        Model::Node* handNode = isLeftHand
            ? player.model->FindNode("mixamorig:LeftHand")
            : player.model->FindNode("mixamorig:RightHand");

        handPos = {
            handNode->worldTransform._41,
            handNode->worldTransform._42,
            handNode->worldTransform._43
        };

        DirectX::XMFLOAT3 targetPoint = isLeftHand ? player.GetPreviousSwingPoint() : player.GetswingPoint();
        DirectX::XMFLOAT3 dir = targetPoint - handPos;

        DirectX::XMVECTOR dirVec = DirectX::XMLoadFloat3(&dir);
        dirVec = DirectX::XMVector3Normalize(dirVec);
        DirectX::XMStoreFloat3(&direction, dirVec);

        position = handPos;
    }
    else
    {
        // ロックオン中の敵が存在するか確認
        Enemy* lockonEnemy = player.GetLockonEnemy();
        if (lockonEnemy != nullptr && lockonEnemy->GetAlive())
        {
            // 糸は手から出るように、右手のNodeを探す
            Model::Node* RightHandPos = player.model->FindNode("mixamorig:RightHand");
            DirectX::XMFLOAT3 pos;
            pos.x = RightHandPos->worldTransform._41;
            pos.y = RightHandPos->worldTransform._42;
            pos.z = RightHandPos->worldTransform._43;

            DirectX::XMFLOAT3 EnemyPos = lockonEnemy->GetPosition();
            DirectX::XMFLOAT3 dir = EnemyPos - pos;
            DirectX::XMVECTOR dirVec = DirectX::XMLoadFloat3(&dir);
            dirVec = DirectX::XMVector3Normalize(dirVec);
            DirectX::XMStoreFloat3(&direction, dirVec);

            position = pos;
        }
        else
        {
            // ロックオン中の敵が存在しない場合、適切な処理を行う
            // 例えば、糸を消すなど
            SceneGame& sceneGame = SceneGame::Instance();
            if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            {
                Destroy();
                sceneGame.UnregisterRenderModel(model.get());
            }
        }
    }

    UpdateTransform();
    model->UpdateTransform(transform);

}

