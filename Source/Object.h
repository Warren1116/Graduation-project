#pragma once
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Enemy.h"

class ObjectBase
{
public:
	ObjectBase() {}
	virtual ~ObjectBase() {}

	// �s��X�V����
	void UpdateTransform();

	// �ʒu�擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// �ʒu�ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	// ��]�擾
	const DirectX::XMFLOAT3& GetAngle()const { return angle; }

	// ��]�ݒ�
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	// �X�P�[���擾
	const DirectX::XMFLOAT3& GetScale()const { return scale; }

	// �X�P�[���ݒ�
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	// ���a�擾
	float GetRadius() const { return radius; }

	// �����擾
	float GetHeight()const { return height; };

protected:

	// �ړ�����
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
	// �R���X�g���N�^
	Start(Enemy* enemy);

	// �f�X�g���N�^
	~Start() override;

	static Start& Instance();

	// �X�V����
	void Update(float elapsedTime, Enemy* enemy);

	// �`�揈��
	void Render(const RenderContext& rc, ModelShader* shader);

private:
	float				moveSpeed = 5.0f;
};

// �S�[��
class Goal :public ObjectBase
{
public:
	// �R���X�g���N�^
	Goal();

	// �f�X�g���N�^
	~Goal() override;

	static Goal& Instance();

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(const RenderContext& rc, ModelShader* shader);

	// �ʒu�ݒ�
	void SetPosition(DirectX::XMFLOAT3 position) { this->position = position; }
};


// ��Q��
class Obstacle :public ObjectBase
{
public:
	// �R���X�g���N�^
	Obstacle();

	// �f�X�g���N�^
	~Obstacle() override;

	// �`�揈��
	void Render(const RenderContext& rc, ModelShader* shader);

	void SetPositionIndex(int index) { this->posIndex = index; }

	int posIndex = -1;
};

// ��Q���}�l�[�W���[
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
	// �R���X�g���N�^
	Terrain();
	// �f�X�g���N�^
	~Terrain() override;

	void SetPositionIndex(int index) { this->posIndex = index; }

	// �`�揈��
	void Render(const RenderContext& rc, ModelShader* shader);
	int posIndex = -1;
};
// ��Q���}�l�[�W���[
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
	// �R���X�g���N�^
	Arrow();
	// �f�X�g���N�^
	~Arrow() override;
	void SetTransfrom(DirectX::XMFLOAT4X4 matrix);
	void SetTransfrom(DirectX::XMMATRIX matrix);
	// �`�揈��
	void Render(const RenderContext& rc, ModelShader* shader);
	float height = 1.0f;
	float radius = 1.0f;
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
};
// ��Q���}�l�[�W���[
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
