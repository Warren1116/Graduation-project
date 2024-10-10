#include "StageMain.h"
#include "Graphics/Graphics.h"
#include "Object.h"
#include "Input/Input.h"
#include "Zombie.h"
#include "PoisonZombie.h"
#include "Item.h"
#include "ItemKey.h"
#include "EnemyManager.h"
#include "ItemManager.h"

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
	stageNum++;

	if (stageNum == 0)
	{
		model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
		position = { 0.0f, 0.0f, 0.0f };
		scale.x = scale.y = scale.z = 1.0f;
		angle.y = DirectX::XMConvertToRadians(180);

		for (int i = 0; i < ROW_COUNT; ++i) {
			for (int j = 0; j < COLUM_COUNT; ++j)
			{
				// インデックス番号計算
				int index = (i * COLUM_COUNT) + j;
				//WayPoint生成
				wayPoint[index] = new WayPoint();

				// WayPointの座標決定
				wayPoint[index]->position = DirectX::XMFLOAT3(i + 0.5f, 1.0f, j + 0.5f);

				// WayPointに紐づくエッジを生成
				for (int k = 0; k < 8; ++k)
				{
					wayPoint[index]->edge[k] = new Edge();
					wayPoint[index]->edge[k]->originPoint = index;
				}
			}
		}
		for (int i = 0; i < ROW_COUNT; ++i) {
			for (int j = 0; j < COLUM_COUNT; ++j)
			{
				// インデックス番号計算
				int index = (i * COLUM_COUNT) + j;
				DestinationPointSet(index);
			}
		}

		Obstacle* obstacle = new Obstacle();
		obstacle->SetPosition(GetIndexWayPoint(0));
		obstacle->SetPositionIndex(0);
		ObstacleManager::Instance().AddObstacle(obstacle);

		int index = 0;
		int obstacleIndex[ROW_COUNT][COLUM_COUNT] =
		{
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
		};
		for (int x = 0; x < ROW_COUNT; ++x)
		{
			for (int y = 0; y < COLUM_COUNT; ++y)
			{
				if (obstacleIndex[x][y] == 0)
				{
					index = (x * COLUM_COUNT) + y;
					SetObstacle(index);
				}
			}
		}
	}


	
}

// デストラクタ
StageMain::~StageMain()
{
	// ステージモデルを破棄
	delete model;
	
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
	return Collision::IntersectRayVsModel(start, end, model, hit);
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

void StageMain::DestinationPointSet(int index)
{
	int x = index % COLUM_COUNT;
	// エッジの行先指定
	// 12時の方向
	// -COLUM_COUNTの値が0以上であれば
	int point = index - COLUM_COUNT;
	if (point >= 0)
	{
		wayPoint[index]->edge[0]->destinationPoint = point;

		// 2点間の距離を計算してエッジのコストに追加
		DirectX::XMVECTOR destinationPoint = DirectX::XMLoadFloat3(&wayPoint[point]->position);
		DirectX::XMVECTOR originPoint = DirectX::XMLoadFloat3(&wayPoint[index]->position);
		DirectX::XMVECTOR cost = DirectX::XMVector3Length(DirectX::XMVectorSubtract(destinationPoint, originPoint));
		wayPoint[index]->edge[0]->cost = DirectX::XMVectorGetX(cost);
	}
	else
	{
		wayPoint[index]->edge[0]->destinationPoint = -1;
		wayPoint[index]->edge[0]->cost = FLT_MAX;
	}

	// 1.5時の方向
	// -COLUM_COUNT + 1の値が0以上であり、なおかつ(j+1)が横幅を超えないとき
	point = index - COLUM_COUNT + 1;
	if (point >= 0 && (x + 1) < COLUM_COUNT)
	{
		wayPoint[index]->edge[1]->destinationPoint = point;
		// 2点間の距離を計算してエッジのコストに追加
		DirectX::XMVECTOR destinationPoint = DirectX::XMLoadFloat3(&wayPoint[point]->position);
		DirectX::XMVECTOR originPoint = DirectX::XMLoadFloat3(&wayPoint[index]->position);
		DirectX::XMVECTOR cost = DirectX::XMVector3Length(DirectX::XMVectorSubtract(destinationPoint, originPoint));
		wayPoint[index]->edge[1]->cost = DirectX::XMVectorGetX(cost);
	}
	else
	{
		wayPoint[index]->edge[1]->destinationPoint = -1;
		wayPoint[index]->edge[1]->cost = FLT_MAX;
	}

	// 3時の方向
	// (j+1)が横幅を超えないとき
	point = index + 1;
	if ((x + 1) < COLUM_COUNT)
	{
		wayPoint[index]->edge[2]->destinationPoint = point;
		// 2点間の距離を計算してエッジのコストに追加
		DirectX::XMVECTOR destinationPoint = DirectX::XMLoadFloat3(&wayPoint[point]->position);
		DirectX::XMVECTOR originPoint = DirectX::XMLoadFloat3(&wayPoint[index]->position);
		DirectX::XMVECTOR cost = DirectX::XMVector3Length(DirectX::XMVectorSubtract(destinationPoint, originPoint));
		wayPoint[index]->edge[2]->cost = DirectX::XMVectorGetX(cost);
	}
	else
	{
		wayPoint[index]->edge[2]->destinationPoint = -1;
		wayPoint[index]->edge[2]->cost = FLT_MAX;
	}
	// 4.5時の方向
	// COLUM_COUNT + 1の値が最大値未満であり、なおかつ(j+1)が横幅を超えないとき
	point = index + COLUM_COUNT + 1;
	if (point < MAX_WAY_POINT && (x + 1) < COLUM_COUNT)
	{
		wayPoint[index]->edge[3]->destinationPoint = point;
		// 2点間の距離を計算してエッジのコストに追加
		DirectX::XMVECTOR destinationPoint = DirectX::XMLoadFloat3(&wayPoint[point]->position);
		DirectX::XMVECTOR originPoint = DirectX::XMLoadFloat3(&wayPoint[index]->position);
		DirectX::XMVECTOR cost = DirectX::XMVector3Length(DirectX::XMVectorSubtract(destinationPoint, originPoint));
		wayPoint[index]->edge[3]->cost = DirectX::XMVectorGetX(cost);
	}
	else
	{
		wayPoint[index]->edge[3]->destinationPoint = -1;
		wayPoint[index]->edge[3]->cost = FLT_MAX;
	}
	// 6時の方向
	// +COLUM_COUNTの値が最大値未満のとき
	point = index + COLUM_COUNT;
	if (point < MAX_WAY_POINT)
	{
		wayPoint[index]->edge[4]->destinationPoint = point;
		// 2点間の距離を計算してエッジのコストに追加
		DirectX::XMVECTOR destinationPoint = DirectX::XMLoadFloat3(&wayPoint[point]->position);
		DirectX::XMVECTOR originPoint = DirectX::XMLoadFloat3(&wayPoint[index]->position);
		DirectX::XMVECTOR cost = DirectX::XMVector3Length(DirectX::XMVectorSubtract(destinationPoint, originPoint));
		wayPoint[index]->edge[4]->cost = DirectX::XMVectorGetX(cost);
	}
	else
	{
		wayPoint[index]->edge[4]->destinationPoint = -1;
		wayPoint[index]->edge[4]->cost = FLT_MAX;
	}
	// 7.5時の方向
	// +COLUM_COUNTの値が最大値未満でjが0より多きいとき
	point = index + COLUM_COUNT - 1;
	if (point < MAX_WAY_POINT && x > 0)
	{
		wayPoint[index]->edge[5]->destinationPoint = point;
		// 2点間の距離を計算してエッジのコストに追加
		DirectX::XMVECTOR destinationPoint = DirectX::XMLoadFloat3(&wayPoint[point]->position);
		DirectX::XMVECTOR originPoint = DirectX::XMLoadFloat3(&wayPoint[index]->position);
		DirectX::XMVECTOR cost = DirectX::XMVector3Length(DirectX::XMVectorSubtract(destinationPoint, originPoint));
		wayPoint[index]->edge[5]->cost = DirectX::XMVectorGetX(cost);
	}
	else
	{
		wayPoint[index]->edge[5]->destinationPoint = -1;
		wayPoint[index]->edge[5]->cost = FLT_MAX;
	}
	// 9時の方向
	// jが0より多きいとき
	point = index - 1;
	if (x > 0)
	{
		wayPoint[index]->edge[6]->destinationPoint = point;
		// 2点間の距離を計算してエッジのコストに追加
		DirectX::XMVECTOR destinationPoint = DirectX::XMLoadFloat3(&wayPoint[point]->position);
		DirectX::XMVECTOR originPoint = DirectX::XMLoadFloat3(&wayPoint[index]->position);
		DirectX::XMVECTOR cost = DirectX::XMVector3Length(DirectX::XMVectorSubtract(destinationPoint, originPoint));
		wayPoint[index]->edge[6]->cost = DirectX::XMVectorGetX(cost);
	}
	else
	{
		wayPoint[index]->edge[6]->destinationPoint = -1;
		wayPoint[index]->edge[6]->cost = FLT_MAX;
	}
	// 10.5時の方向
	// jが0より多きいとき
	point = index - COLUM_COUNT - 1;
	if (point >= 0 && x > 0)
	{
		wayPoint[index]->edge[7]->destinationPoint = point;
		// 2点間の距離を計算してエッジのコストに追加
		DirectX::XMVECTOR destinationPoint = DirectX::XMLoadFloat3(&wayPoint[point]->position);
		DirectX::XMVECTOR originPoint = DirectX::XMLoadFloat3(&wayPoint[index]->position);
		DirectX::XMVECTOR cost = DirectX::XMVector3Length(DirectX::XMVectorSubtract(destinationPoint, originPoint));
		wayPoint[index]->edge[7]->cost = DirectX::XMVectorGetX(cost);
	}
	else
	{
		wayPoint[index]->edge[7]->destinationPoint = -1;
		wayPoint[index]->edge[7]->cost = FLT_MAX;
	}
}

void StageMain::DrawDebugPrimitive(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	LineRenderer* lineRenderer = Graphics::Instance().GetLineRenderer();
	
	for (int i = 0; i <= ROW_COUNT; ++i)
	{
		// x軸罫線
		lineRenderer->AddVertex(DirectX::XMFLOAT3(i * 1.0f, 0.1f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		lineRenderer->AddVertex(DirectX::XMFLOAT3(i * 1.0f, 0.1f, COLUM_COUNT), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	for (int i = 0; i <= COLUM_COUNT; ++i)
	{
		// z軸罫線
		lineRenderer->AddVertex(DirectX::XMFLOAT3(0, 0.1f, i * 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		lineRenderer->AddVertex(DirectX::XMFLOAT3(ROW_COUNT, 0.1f, i * 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	lineRenderer->Render(dc, view, projection);

	// 各ウェイポイントからのエッジを表示
	for (int i = 0; i < MAX_WAY_POINT; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			// エッジが生成されていなければcontinue
			if (wayPoint[i]->edge[j] == nullptr)continue;
			// 
			if (wayPoint[i]->edge[j]->destinationPoint < 0 || wayPoint[i]->edge[j]->destinationPoint >= MAX_WAY_POINT)continue;

			DirectX::XMFLOAT3 originPosition = GetIndexWayPoint(wayPoint[i]->edge[j]->originPoint);
			DirectX::XMFLOAT3 destinationPosition = GetIndexWayPoint(wayPoint[i]->edge[j]->destinationPoint);
			lineRenderer->AddVertex(originPosition, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
			lineRenderer->AddVertex(destinationPosition, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		}
	}
	lineRenderer->Render(dc, view, projection);


	// ウェイポイントを描画
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	// ウェイポイントを描画
	for (int i = 0; i < MAX_WAY_POINT; ++i)
	{
		debugRenderer->DrawSphere(wayPoint[i]->position, 0.1f, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
}

void StageMain::SetObstacle(int index)
{
	for (int i = 0; i < EdgeNo; ++i)
	{

		int point = wayPoint[index]->edge[i]->destinationPoint;
		if (point < 0) continue;

		wayPoint[index]->edge[i]->destinationPoint = -1;

		for (int j = 0; j < EdgeNo; ++j)
		{
			if (wayPoint[point]->edge[j]->destinationPoint == index)
			{
				wayPoint[point]->edge[j]->destinationPoint = -1;
				wayPoint[point]->edge[j]->cost = FLT_MAX;
			}
		}
		wayPoint[i]->obstacleFlag = true;
	}
}

void StageMain::DrawDebugGUI()
{
		ImGui::DragFloat3("position", &position.x);
}

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