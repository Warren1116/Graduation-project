#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "Graphics/ModelResource.h"

// ���f��
class Model
{
public:
	Model(const char* filename);
	~Model() {}

	struct Node
	{
		const char*			name;
		Node*				parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	localTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		DirectX::XMFLOAT4X4	globalTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		DirectX::XMFLOAT4X4	worldTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

		std::vector<Node*>	children;
	};

	enum class AlphaMode
	{
		Opaque,
		Mask,
		Blend
	};

	struct Material
	{
		std::string			name;
		std::string			baseTextureFileName;
		std::string			normalTextureFileName;
		std::string			emissiveTextureFileName;
		std::string			occlusionTextureFileName;
		std::string			metalnessRoughnessTextureFileName;
		DirectX::XMFLOAT4	baseColor = { 1, 1, 1, 1 };
		DirectX::XMFLOAT3	emissiveColor = { 1, 1, 1 };
		float				metalness = 0.0f;
		float				roughness = 0.0f;
		float				occlusionStrength = 0.0f;
		float				alphaCutoff = 0.5f;
		AlphaMode			alphaMode = AlphaMode::Opaque;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	baseMap;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	normalMap;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	emissiveMap;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	occlusionMap;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	metalnessRoughnessMap;


	};

	struct Vertex
	{
		DirectX::XMFLOAT3		position = { 0, 0, 0 };
		DirectX::XMFLOAT3		normal = { 0, 0, 0 };
		DirectX::XMFLOAT4		tangent = { 0, 0, 0, 1 };
		DirectX::XMFLOAT2		texcoord = { 0, 0 };
		DirectX::XMFLOAT4		boneWeight = { 1, 0, 0, 0 };
		DirectX::XMUINT4		boneIndex = { 0, 0, 0, 0 };


	};

	struct Bone
	{
		int						nodeIndex;
		DirectX::XMFLOAT4X4		offsetTransform;
		Node* node = nullptr;

	};

	struct Mesh
	{
		std::vector<Vertex>		vertices;
		std::vector<uint32_t>	indices;
		std::vector<Bone>		bones;
		int			nodeIndex = 0;
		int			materialIndex = 0;
		Material* material = nullptr;
		Node* node = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer>	vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>	indexBuffer;

	};

	struct VectorKeyframe
	{
		float					seconds;
		DirectX::XMFLOAT3		value;

	};

	struct QuaternionKeyframe
	{
		float					seconds;
		DirectX::XMFLOAT4		value;

	};

	struct NodeAnim
	{
		std::vector<VectorKeyframe>		positionKeyframes;
		std::vector<QuaternionKeyframe>	rotationKeyframes;
		std::vector<VectorKeyframe>		scaleKeyframes;

	};

	struct Animation
	{
		const char* name;
		float						secondsLength;
		std::vector<NodeAnim>		nodeAnims;


	};

	struct NodePose
	{
		DirectX::XMFLOAT3	position = { 0, 0, 0 };
		DirectX::XMFLOAT4	rotation = { 0, 0, 0, 1 };
		DirectX::XMFLOAT3	scale = { 1, 1, 1 };
	};

	// �s��v�Z
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);

	// �m�[�h���X�g�擾
	const std::vector<Node>& GetNodes() const { return nodes; }
	std::vector<Node>& GetNodes() { return nodes; }

	// ���\�[�X�擾
	const ModelResource* GetResource() const { return resource.get(); }

	void UpdateAnimation(float elapsedTime);

	void PlayAnimation(int index, bool loop, float blendSeconds = 0.2f);

	bool IsPlayAnimation() const;

	// �A�j���[�V�����f�[�^�擾
	const std::vector<Animation>& GetAnimations() const { return animations; }

	// �A�j���[�V�����C���f�b�N�X�擾
	int GetAnimationIndex(const char* name) const;

	// �m�[�h�C���f�b�N�X�擾
	int GetNodeIndex(const char* name) const;

	//�m�[�h����
	Node* FindNode(const char* name);

	//���݂̃A�j���[�V�����Đ����Ԏ擾
	float GetCurrentAnimationSeconds() const { return currentAnimationSeconds; }

	// �A�j���[�V�����v�Z
	void ComputeAnimation(int animationIndex, int nodeIndex, float time, NodePose& nodePose) const;
	void ComputeAnimation(int animationIndex, float time, std::vector<NodePose>& nodePoses) const;

	// �m�[�h�|�[�Y�ݒ�
	void SetNodePoses(const std::vector<NodePose>& nodePoses);

	// �m�[�h�|�[�Y�擾
	void GetNodePoses(std::vector<NodePose>& nodePoses) const;



private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				nodes;
	std::vector<Animation>	animations;

	int currentAnimationIndex = -1;
	float currentAnimationSeconds = 0.0f;
	bool animationLoopFlag = false;
	bool animotionFirstFlag = false;
	bool animationEndFlag = false;
	float animationBlendTime = 0.0f;
	float animationBlendSeconds = 0.0f;
	int rootMotionNodeIndex = -1;


};
