#include "StageMain.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "EnemyManager.h"

static  StageMain* instance = nullptr;
int stageNum = -1;

// インスタンス取得
StageMain& StageMain::Instance()
{
    return *instance;
}

int StageMain::GetStageNum()
{
    return stageNum;
}

void StageMain::SetStageNum(int num)
{
    stageNum = num;
}

// コンストラクタ
StageMain::StageMain()
{
    instance = this;

    // ステージモデルの読み込み
    model = std::make_unique<Model>("Data/Model/ExampleStage/ExampleStage.mdl");
    position = { 0.0f, 0.0f, 0.0f };
    scale.x = scale.y = scale.z = 1.0f;

    //model = std::make_unique<Model>("Data/Model/City/city_fixed3.mdl");
    //position = { 10.0f, -50.0f, 25.0f };
    //scale.x = scale.y = scale.z = 0.04f;
    angle.y = DirectX::XMConvertToRadians(180);

    stageNum++;

}

// デストラクタ
StageMain::~StageMain()
{
}

// 更新処理
void StageMain::Update(float elapsedTime)
{
    UpdateTransform();
    model->UpdateTransform(transform);
}

// レイキャスト
bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model.get(), hit);
}

DirectX::XMFLOAT3 StageMain::GetIndexWayPoint(int index)
{
    return wayPoint[index]->position;
}

int StageMain::NearWayPointIndex(DirectX::XMFLOAT3 target)
{
    float minLength = FLT_MAX;
    float length = 0.0f;
    int index = -1;

    DirectX::XMVECTOR targetPos = DirectX::XMLoadFloat3(&target);

    for (int i = 0; i < MAX_WAY_POINT; ++i)
    {
        DirectX::XMVECTOR point = DirectX::XMLoadFloat3(&(wayPoint[i]->position));

        // 距離を求める
        DirectX::XMVECTOR vector = DirectX::XMVectorSubtract(targetPos, point);
        DirectX::XMVECTOR vectorLength = DirectX::XMVector3Length(vector);
        DirectX::XMStoreFloat(&length, vectorLength);

        // 求めた距離が保存しているものより小さければ
        if (minLength > length)
        {
            // 値を更新
            minLength = length;
            index = i;
        }
    }
    return index;
}


//void StageMain::DrawDebugGUI()
//{
//    ImGui::DragFloat3("position", &position.x);
//}

//void StageMain::SetEnemyObject(int index)
//{
//	EnemyManager& enemyManager = EnemyManager::Instance();
//	ItemManager& itemManager = ItemManager::Instance();
//	
//	switch (index)
//	{
//		case 0:
//		{
//			Zombie* zombies[11];
//			for (int i = 0; i < 11; ++i)
//			{
//				zombies[i] = new Zombie();
//				enemyManager.Register(zombies[i]);
//			}
//
//			zombies[0]->SetPosition(DirectX::XMFLOAT3(15.0f, 0.0f, 4.0f));
//			zombies[1]->SetPosition(DirectX::XMFLOAT3(22.0f, 0.0f, 4.0f));
//			zombies[2]->SetPosition(DirectX::XMFLOAT3(30.0f, 0.0f, 4.0f));
//			zombies[3]->SetPosition(DirectX::XMFLOAT3(37.0f, 0.0f, 4.0f));
//			zombies[4]->SetPosition(DirectX::XMFLOAT3(45.0f, 0.0f, 4.0f));
//			zombies[5]->SetPosition(DirectX::XMFLOAT3(44.0f, 0.0f, 20.0f));
//			zombies[6]->SetPosition(DirectX::XMFLOAT3(37.0f, 0.0f, 20.0f));
//			zombies[7]->SetPosition(DirectX::XMFLOAT3(15.0f, 0.0f, 15.0f));
//			zombies[8]->SetPosition(DirectX::XMFLOAT3(25.0f, 0.0f, 15.0f));
//			zombies[9]->SetPosition(DirectX::XMFLOAT3(35.0f, 0.0f, 15.0f));
//
//			for (int i = 0; i < 10; ++i)
//			{
//				zombies[i]->AddStart(zombies[i]);
//			}
//
//			ItemKey* items[3];
//			for (int i = 0; i < 3; ++i)
//			{
//				items[i] = new ItemKey();
//				items[i]->SetAngle({ DirectX::XMConvertToRadians(90), DirectX::XMConvertToRadians(90), DirectX::XMConvertToRadians(90) });
//				itemManager.Register(items[i]);
//			}
//			items[0]->SetPosition(DirectX::XMFLOAT3(15.0f, 0.0f, 15.0f));
//			items[1]->SetPosition(DirectX::XMFLOAT3(44.0f, 0.0f, 4.0f));
//			items[2]->SetPosition(DirectX::XMFLOAT3(3.0f, 0.0f, 13.5f));
//			break;
//		}
//
//		case 1:
//		{
//			PoisonZombie* poisonZombie = new PoisonZombie();
//			poisonZombie->SetPosition(DirectX::XMFLOAT3(20.0f, 0.0f, 1.5f));
//			enemyManager.Register(poisonZombie);
//
//			Zombie* zombies[10];
//			for (int i = 0; i < 10; ++i)
//			{
//				zombies[i] = new Zombie();
//				enemyManager.Register(zombies[i]);
//			}
//
//			zombies[0]->SetPosition(DirectX::XMFLOAT3(38.0f, 0.0f, 26.0f));
//			zombies[1]->SetPosition(DirectX::XMFLOAT3(20.0f, 0.0f, 25.0f));
//			zombies[2]->SetPosition(DirectX::XMFLOAT3(6.0f, 0.0f, 25.0f));
//			zombies[3]->SetPosition(DirectX::XMFLOAT3(8.0f, 0.0f, 18.0f));
//			zombies[4]->SetPosition(DirectX::XMFLOAT3(25.0f, 0.0f, 9.0f));
//			zombies[5]->SetPosition(DirectX::XMFLOAT3(34.0f, 0.0f, 9.0f));
//			zombies[6]->SetPosition(DirectX::XMFLOAT3(49.0f, 0.0f, 13.5f));
//			zombies[7]->SetPosition(DirectX::XMFLOAT3(15.0f, 0.0f, 15.0f));
//			zombies[8]->SetPosition(DirectX::XMFLOAT3(36.0f, 0.0f, 1.5f));
//			zombies[9]->SetPosition(DirectX::XMFLOAT3(33.0f, 0.0f, 1.5f));
//
//			for (int i = 0; i < 10; ++i)
//			{
//				zombies[i]->AddStart(zombies[i]);
//			}
//
//			ItemKey* items[3];
//			for (int i = 0; i < 3; ++i)
//			{
//				items[i] = new ItemKey();
//				items[i]->SetAngle({ DirectX::XMConvertToRadians(90), DirectX::XMConvertToRadians(90), DirectX::XMConvertToRadians(90) });
//				itemManager.Register(items[i]);
//			}
//			items[0]->SetPosition(DirectX::XMFLOAT3(33.0f, 0.0f, 3.0f));
//			items[1]->SetPosition(DirectX::XMFLOAT3(8.0f, 0.0f, 18.0f));
//			items[2]->SetPosition(DirectX::XMFLOAT3(38.0f, 0.0f, 26.0f));
//			break;
//		}
//	}
//}