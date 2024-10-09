#pragma once
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Enemy.h"

class ObjectBase
{
public:
	ObjectBase() {}
	virtual ~ObjectBase() {}

	// 行列更新処理
	void UpdateTransform();

	// 位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// 位置設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	// 回転取得
	const DirectX::XMFLOAT3& GetAngle()const { return angle; }

	// 回転設定
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	// スケール取得
	const DirectX::XMFLOAT3& GetScale()const { return scale; }

	// スケール設定
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	// 半径取得
	float GetRadius() const { return radius; }

	// 高さ取得
	float GetHeight()const { return height; };

protected:

	// 移動処理
	void Move(float vx, float vz, float speed);

protected:
	DirectX::XMFLOAT3	position = { 0,0,0 };
	DirectX::XMFLOAT3	angle = { 0,0,0, };
	DirectX::XMFLOAT3	scale = { 1,1,1 };
	DirectX::XMFLOAT4X4	transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	float	radius = 0.3f;
	float	height = 2.0f;

private:
};

// Start
class Start :public ObjectBase
{
public:
	// コンストラクタ
	Start(Enemy* enemy);

	// デストラクタ
	~Start() override;

	static Start& Instance();

	// 更新処理
	void Update(float elapsedTime, Enemy* enemy);

	// 描画処理
	void Render(const RenderContext& rc, ModelShader* shader);

private:
	float				moveSpeed = 5.0f;
};

// ゴール
class Goal :public ObjectBase
{
public:
	// コンストラクタ
	Goal();

	// デストラクタ
	~Goal() override;

	static Goal& Instance();

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(const RenderContext& rc, ModelShader* shader);

	// 位置設定
	void SetPosition(DirectX::XMFLOAT3 position) { this->position = position; }
};


// 障害物
class Obstacle :public ObjectBase
{
public:
	// コンストラクタ
	Obstacle();

	// デストラクタ
	~Obstacle() override;

	// 描画処理
	void Render(const RenderContext& rc, ModelShader* shader);

	void SetPositionIndex(int index) { this->posIndex = index; }

	int posIndex = -1;
};

// 障害物マネージャー
class ObstacleManager
{

private:
	ObstacleManager() {};
	~ObstacleManager() {};
public:
	static ObstacleManager& Instance()
	{
		static ObstacleManager instance;
		return instance;
	}
	void Clear();
	void Render(const RenderContext& rc, ModelShader* shader);
	void AddObstacle(Obstacle* terrain);
	void EraseObstacle(int index);
	int GetSearchObstacleIndex(int index);
	std::vector<Obstacle*> obstacles;
	std::vector<Obstacle*> removes;
};

class Terrain :public ObjectBase
{
public:
	// コンストラクタ
	Terrain();
	// デストラクタ
	~Terrain() override;

	void SetPositionIndex(int index) { this->posIndex = index; }

	// 描画処理
	void Render(const RenderContext& rc, ModelShader* shader);
	int posIndex = -1;
};
// 障害物マネージャー
class TerrainManager
{
private:
	TerrainManager() {}
	~TerrainManager() {}
public:
	static TerrainManager& Instance()
	{
		static TerrainManager instance;
		return instance;
	}
	void Clear();
	void Render(const RenderContext& rc, ModelShader* shader);
	void AddTerrain(Terrain* terrain);
	void EraseTerrain(int index);
	int GetSearchTerrainIndex(int index);
	std::vector<Terrain*> terrains;
	std::vector<Terrain*> removes;

	int posIndex = -1;
};

class Arrow :public ObjectBase
{
public:
	// コンストラクタ
	Arrow();
	// デストラクタ
	~Arrow() override;
	void SetTransfrom(DirectX::XMFLOAT4X4 matrix);
	void SetTransfrom(DirectX::XMMATRIX matrix);
	// 描画処理
	void Render(const RenderContext& rc, ModelShader* shader);
	float height = 1.0f;
	float radius = 1.0f;
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
};
// 障害物マネージャー
class ArrowManager
{
private:
	ArrowManager() {}
	~ArrowManager() {}
public:
	static ArrowManager& Instance()
	{
		static ArrowManager instance;
		return instance;
	}

	void Clear();
	void Render(const RenderContext& rc, ModelShader* shader);
	void AddArrow(Arrow* arrow);
	std::vector<Arrow*> arrows;
	std::vector<Arrow*> removes;
};
