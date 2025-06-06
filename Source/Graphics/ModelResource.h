#pragma once

#include <string>
#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>


class ModelResource
{
public:
	ModelResource() {}
	virtual ~ModelResource() {}

	using NodeId = UINT64;

	struct Node
	{
		NodeId              id;
		std::string         name;
		std::string         path;
		int                 parentIndex = -1;
		Node* parent = nullptr;
		std::vector<Node*>  children;

		DirectX::XMFLOAT3   scale = { 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT4   rotate = { 0.0f, 0.0f, 0.0f, 1.0f };
		DirectX::XMFLOAT3   translate = { 0.0f, 0.0f, 0.0f };

		DirectX::XMFLOAT4X4 localTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		DirectX::XMFLOAT4X4 globalTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		DirectX::XMFLOAT4X4 worldTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

		void UpdateTransform()
		{
			DirectX::XMStoreFloat4x4(&localTransform,
				DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
				DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotate)) *
				DirectX::XMMatrixTranslation(translate.x, translate.y, translate.z)
			);

			if (parent)
			{
				DirectX::XMStoreFloat4x4(&globalTransform,
					DirectX::XMLoadFloat4x4(&localTransform) * DirectX::XMLoadFloat4x4(&parent->globalTransform));
			}
			else
			{
				globalTransform = localTransform;
			}

			for (auto* child : children)
			{
				child->UpdateTransform();
			}
		}

		void AddChild(Node* child)
		{
			child->parent = this;
			children.push_back(child);
		}

		template<class Archive>
		void serialize(Archive& archive, int version);

	};

	struct Material
	{
		std::string			name;
		std::string			textureFilename;
		int					shaderId = 0;
		DirectX::XMFLOAT4	color = { 0.8f, 0.8f, 0.8f, 1.0f };
		struct Phong
		{
			DirectX::XMFLOAT4 ka = DirectX::XMFLOAT4(1, 1, 1, 1);
			DirectX::XMFLOAT4 kd = DirectX::XMFLOAT4(1, 1, 1, 1);
			DirectX::XMFLOAT4 ks = DirectX::XMFLOAT4(1, 1, 1, 1);
			float shiness;
			Phong() {}
		} phong;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuse_map;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normal_map;


		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Subset
	{
		UINT		startIndex = 0;
		UINT		indexCount = 0;
		int			materialIndex = 0;

		Material* material = nullptr;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Vertex
	{
		DirectX::XMFLOAT3	position = { 0, 0, 0 };
		DirectX::XMFLOAT3	normal = { 0, 0, 0 };
		DirectX::XMFLOAT3	tangent = { 0, 0, 0 };
		DirectX::XMFLOAT2	texcoord = { 0, 0 };
		DirectX::XMFLOAT4	color = { 1, 1, 1, 1 };
		DirectX::XMFLOAT4	boneWeight = { 1, 0, 0, 0 };
		DirectX::XMUINT4	boneIndex = { 0, 0, 0, 0 };

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Mesh
	{
		std::vector<Vertex>						vertices;
		std::vector<UINT>						indices;
		std::vector<Subset>						subsets;

		int										nodeIndex;
		std::vector<int>						nodeIndices;
		std::vector<DirectX::XMFLOAT4X4>		offsetTransforms;

		DirectX::XMFLOAT3						boundsMin;
		DirectX::XMFLOAT3						boundsMax;

		Node* node = nullptr;

		Microsoft::WRL::ComPtr<ID3D11Buffer>	vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>	indexBuffer;



		template<class Archive>
		void serialize(Archive& archive, int version);
	};





	struct NodeKeyData
	{
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Keyframe
	{
		float						seconds;
		std::vector<NodeKeyData>	nodeKeys;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};
	struct Animation
	{
		std::string					name;
		float						secondsLength;
		std::vector<Keyframe>		keyframes;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	// 各種データ取得
	const std::vector<Mesh>& GetMeshes() const { return meshes; }
	const std::vector<Node>& GetNodes() const { return nodes; }
	const std::vector<Animation>& GetAnimations() const { return animations; }
	const std::vector<Material>& GetMaterials() const { return materials; }

	// 読み込み
	void Load(ID3D11Device* device, const char* filename);

protected:
	// モデルセットアップ
	void BuildModel(ID3D11Device* device, const char* dirname);

	// テクスチャ読み込み
	HRESULT LoadTexture(ID3D11Device* device, const char* filename, const char* suffix, bool dummy, ID3D11ShaderResourceView** srv, UINT dummy_color = 0xFFFFFFFF);

	// シリアライズ
	void Serialize(const char* filename);

	// デシリアライズ
	void Deserialize(const char* filename);

	// ノードインデックスを取得する
	int FindNodeIndex(NodeId nodeId) const;

protected:
	std::vector<Node>		nodes;
	std::vector<Material>	materials;
	std::vector<Mesh>		meshes;
	std::vector<Animation>	animations;
};
