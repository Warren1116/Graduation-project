#include "StageMain.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "EnemyManager.h"
#include "Camera.h"
#include "Player.h"
#include <algorithm>


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
    ID3D11Device* device = Graphics::Instance().GetDevice();

    instance = this;
    // ステージモデルの読み込み
    //model = std::make_unique<Model>("Data/Model/ExampleStage/a.mdl");

    model = std::make_unique<Model>("Data/Model/ExampleStage/ExampleStage.mdl");
    position = { 0.0f, 0.0f, 0.0f };
    scale.x = scale.y = scale.z = 1.0f;


    angle.y = DirectX::XMConvertToRadians(0);
    //angle.y = DirectX::XMConvertToRadians(180);


    DirectX::XMVECTOR VolumeMin = DirectX::XMVectorReplicate(FLT_MAX);
    DirectX::XMVECTOR VolumeMax = DirectX::XMVectorReplicate(-FLT_MAX);

    const ModelResource* resource = model->GetResource();
    const std::vector<Model::Node>& nodes = model->GetNodes();

    // 頂点データをワールド空間変換し、三角形データを作成
    //for (const Model::Mesh& mesh : model->GetMeshes())
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        const Model::Node& node = nodes.at(mesh.nodeIndex);
        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
        for (size_t i = 0; i < mesh.indices.size(); i += 3)
        {
            // 頂点データをワールド空間変換
            uint32_t a = mesh.indices.at(i + 0);
            uint32_t b = mesh.indices.at(i + 1);
            uint32_t c = mesh.indices.at(i + 2);
            DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&mesh.vertices.at(a).position);
            DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&mesh.vertices.at(b).position);
            DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&mesh.vertices.at(c).position);
            A = DirectX::XMVector3Transform(A, WorldTransform);
            B = DirectX::XMVector3Transform(B, WorldTransform);
            C = DirectX::XMVector3Transform(C, WorldTransform);

            // 法線ベクトルを算出
            DirectX::XMVECTOR N = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(B, A), DirectX::XMVectorSubtract(C, A));
            N = DirectX::XMVector3Normalize(N);
            if (DirectX::XMVector3Equal(N, DirectX::XMVectorZero()))
            {
                // 面を構成できない場合は除外
                continue;
            }

            // 三角形データを格納
            CollisionMesh::Triangle& triangle = collisionMesh.triangles.emplace_back();
            DirectX::XMStoreFloat3(&triangle.positions[0], A);
            DirectX::XMStoreFloat3(&triangle.positions[1], B);
            DirectX::XMStoreFloat3(&triangle.positions[2], C);
            DirectX::XMStoreFloat3(&triangle.normal, N);

            // モデル全体のAABBを計測
            VolumeMin = DirectX::XMVectorMin(VolumeMin, A);
            VolumeMin = DirectX::XMVectorMin(VolumeMin, B);
            VolumeMin = DirectX::XMVectorMin(VolumeMin, C);
            VolumeMax = DirectX::XMVectorMax(VolumeMax, A);
            VolumeMax = DirectX::XMVectorMax(VolumeMax, B);
            VolumeMax = DirectX::XMVectorMax(VolumeMax, C);

        }
        DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(180));
        WorldTransform = WorldTransform * rotationMatrix;


    }

    // モデル全体のAABB
    DirectX::XMStoreFloat3(&volumeMin, VolumeMin);
    DirectX::XMStoreFloat3(&volumeMax, VolumeMax);

    // モデル全体のAABBからXZ平面に指定のサイズで分割されたコリジョンエリアを作成する
    {
        const int cellSize = 4;
        //エリアを作成していく。
        for (float x = volumeMin.x; x < volumeMax.x; x += cellSize)
        {
            for (float z = volumeMin.z; z < volumeMax.z; z += cellSize)
            {
                CollisionMesh::Area area;
                //AABBを作成
                area.boundingBox.Center.x = x + cellSize / 2;
                area.boundingBox.Center.y = volumeMin.y + (volumeMax.y - volumeMin.y) / 2;
                area.boundingBox.Center.z = z + cellSize / 2;
                area.boundingBox.Extents.x = cellSize / 2;
                area.boundingBox.Extents.y = (volumeMax.y - volumeMin.y) / 2;
                area.boundingBox.Extents.z = cellSize / 2;

                //内包するポリゴンを収集する
                int triangleIndex = 0;
                for (auto& triangle : collisionMesh.triangles)
                {
                    DirectX::XMVECTOR V1, V2, V3;
                    V1 = DirectX::XMLoadFloat3(&triangle.positions[0]);
                    V2 = DirectX::XMLoadFloat3(&triangle.positions[1]);
                    V3 = DirectX::XMLoadFloat3(&triangle.positions[2]);
                    if (area.boundingBox.Intersects(V1, V2, V3))
                    {
                        area.triangleIndices.emplace_back(triangleIndex);
                    }
                    ++triangleIndex;
                }
                collisionMesh.areas.emplace_back(area);
            }
        }
    }

}

// デストラクタ
StageMain::~StageMain()
{
}

// 更新処理
void StageMain::Update(float elapsedTime)
{
    DirectX::XMFLOAT3 playerPos = Player::Instance().GetPosition();

    if (playerPos.x < volumeMin.x || playerPos.x > volumeMax.x
        || playerPos.y < volumeMin.y || playerPos.y > volumeMax.y
        || playerPos.z < volumeMin.z || playerPos.z > volumeMax.z)
    {
        DirectX::XMFLOAT3 clampedPos = playerPos;
        clampedPos.x = std::clamp(playerPos.x, volumeMin.x, volumeMax.x);
        clampedPos.z = std::clamp(playerPos.z, volumeMin.z, volumeMax.z);
        Player::Instance().SetPosition(clampedPos);
    }
    UpdateTransform();
    model->UpdateTransform(transform);
}

void StageMain::DrawDebugPrimitive(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    RenderContext rc;
    Camera& camera = Camera::Instance();

    DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();

    // バウンディングボックス描画
    const DirectX::XMFLOAT4 boxColor = { 0, 1, 1, 1 };
    const DirectX::XMFLOAT3 boxAngle = { 0, 0, 0 };

    for (CollisionMesh::Area& area : collisionMesh.areas)
    {
        DirectX::XMFLOAT3 len = area.boundingBox.Extents;
        len.x *= 2.0f;
        len.y *= 2.0f;
        len.z *= 2.0f;
        debugRender->DrawBox(area.boundingBox.Center, boxAngle, len, boxColor);
    }

}


// レイキャスト
bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model.get(), hit);
}

