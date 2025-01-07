#include "SwingWeb.h"
#include "SceneGame.h"
#include "ProjectileWall.h"
#include "Mathf.h"

SwingWeb* SwingWeb::instance = nullptr;
SwingWeb* SwingWeb::instanceLeft = nullptr;
SwingWeb* SwingWeb::instanceRight = nullptr;

// �R���X�g���N�^
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


    // �\���T�C�Y
    scale.x = scale.y = 0.0008f;
    scale.z = 0.5f;
    radius = 0.3f;

}

// �f�X�g���N�^
SwingWeb::~SwingWeb()
{

}

// �X�V����
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
        // ���b�N�I�����̓G�����݂��邩�m�F
        Enemy* lockonEnemy = player.GetLockonEnemy();
        if (lockonEnemy != nullptr && lockonEnemy->GetAlive())
        {
            // ���͎肩��o��悤�ɁA�E���Node��T��
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
            // ���b�N�I�����̓G�����݂��Ȃ��ꍇ�A�K�؂ȏ������s��
            // �Ⴆ�΁A���������Ȃ�
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

