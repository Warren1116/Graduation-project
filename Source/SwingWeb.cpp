#include "SwingWeb.h"
#include "SceneGame.h"
#include "ProjectileWall.h"
#include "Mathf.h"

SwingWeb* SwingWeb::instanceLeft = nullptr;
SwingWeb* SwingWeb::instanceRight = nullptr;

// �R���X�g���N�^
SwingWeb::SwingWeb(ProjectileManager* manager, bool leftHand) : Projectile(manager), isLeftHand(leftHand)
{

    if (isLeftHand)
    {
        instanceLeft = this;
    }
    else
    {
        instanceRight = this;
    }

    // ���f���ǂݍ���
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
        if (player.GetState() != Player::State::Swing && player.GetState() != Player::State::SwingToKick)
        {
            SceneGame& sceneGame = SceneGame::Instance();
            if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            {
                Destroy();
                sceneGame.UnregisterRenderModel(model.get());
            }
            
        }

        //�@��̈ʒu���擾
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

    
    UpdateTransform();
    model->UpdateTransform(transform);

}

