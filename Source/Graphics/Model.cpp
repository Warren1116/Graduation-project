#include "Graphics/Graphics.h"
#include "Graphics/Model.h"
#include "ResourceManager.h"

// �R���X�g���N�^
Model::Model(const char* filename)
{
	// ���\�[�X�ǂݍ���
	//resource = std::make_shared<ModelResource>();
	//resource->Load(Graphics::Instance().GetDevice(), filename);
	resource = ResourceManager::Instance().LoadModelResource(filename);

	// �m�[�h
	const std::vector<ModelResource::Node>& resNodes = resource->GetNodes();

	nodes.resize(resNodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = resNodes.at(nodeIndex);
		auto&& dst = nodes.at(nodeIndex);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}

	// �s��v�Z
	const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UpdateTransform(transform);

}

// �ϊ��s��v�Z
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ���[�J���s��Z�o
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ���[���h�s��Z�o
		DirectX::XMMATRIX ParentTransform;
		if (node.parent != nullptr)
		{
			ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
		}
		else
		{
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// �v�Z���ʂ��i�[
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

int Model::GetAnimationIndex(const char* name) const
{
	for (size_t animationIndex = 0; animationIndex < animations.size(); ++animationIndex)
	{
		if (animations.at(animationIndex).name == name)
		{
			return static_cast<int>(animationIndex);
		}
	}
	return -1;
}

void Model::UpdateAnimation(float elapsedTime)
{
	//�Đ����łȂ��Ȃ珈�����Ȃ�
	if (!IsPlayAnimation())return;

	if (animationEndFlag)
	{
		animationEndFlag = false;
		currentAnimationIndex = -1;
		return;
	}

	//�u�����h���̌v�Z
	float blendRate = 1.0f;

	if (animationBlendTime < animationBlendSeconds)
	{
		animationBlendTime += elapsedTime;
		if (animationBlendTime >= animationBlendSeconds)
		{
			animationBlendTime = animationBlendSeconds;
		}
		blendRate = animationBlendTime / animationBlendSeconds;
		blendRate *= blendRate;
	}


	//�w��̃A�j���[�V�����f�[�^���擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	//�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());

	for (int keyIndex = 0; keyIndex < keyCount -1; ++keyIndex)
	{
		//���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimationSeconds >= keyframe0.seconds &&
			currentAnimationSeconds < keyframe1.seconds)
		{
			//�Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
			float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				//�Q�̃L�[�t���[���Ԃ̕⊮�v�Z
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];


				//�u�����h�⊮����
				if (blendRate < 1.0f)
				{
					//���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕⊮
					DirectX::XMVECTOR Key0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR Key1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR Rotation = DirectX::XMQuaternionSlerp(Key0, Key1, blendRate);
					DirectX::XMStoreFloat4(&node.rotate, Rotation);

					Key0 = DirectX::XMLoadFloat3(&node.translate);
					Key1 = DirectX::XMLoadFloat3(&key1.translate);
					DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Key0, Key1, blendRate);
					DirectX::XMStoreFloat3(&node.translate, Position);

					Key0 = DirectX::XMLoadFloat3(&node.scale);
					Key1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR Scale = DirectX::XMVectorLerp(Key0, Key1, blendRate);
					DirectX::XMStoreFloat3(&node.scale, Scale);


				}
				//�ʏ�̌v�Z
				else
				{
					//�O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);
				
					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);
				
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

			}
			break;
		}

	}

	//���Ԍo��
	currentAnimationSeconds += elapsedTime;

	//�Đ����Ԃ��I�[���Ԃ𒴂�����
	if (currentAnimationSeconds >= animation.secondsLength)
	{
		if (animationLoopFlag)
		{
			currentAnimationSeconds -= animation.secondsLength;
		}
		//�Đ����Ԃ������߂�
		else
		{
			currentAnimationSeconds -= animation.secondsLength;
			animationEndFlag = true;
		}
	}

}


void Model::PlayAnimation(int index, bool loop, float blendSeconds)
{
	currentAnimationIndex = index;
	currentAnimationSeconds = 0.0f;
	animationLoopFlag = loop;
	animationEndFlag = false;
	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;
}

bool Model::IsPlayAnimation() const
{
	if (currentAnimationIndex < 0) return false;
	if (currentAnimationIndex >= resource->GetAnimations().size()) return false;
	return true;
}

int Model::GetNodeIndex(const char* name) const
{
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		if (nodes.at(nodeIndex).name == name)
		{
			return static_cast<int>(nodeIndex);
		}
	}
	return -1;
}

Model::Node* Model::FindNode(const char* name)
{
	//�S�Ẵm�[�h�𑍓�����Ŗ��O��r����
	for (auto& node : nodes)
	{
		if (std::strcmp(node.name, name) == 0)
		{
			return &node;
		}
	}

	//������Ȃ�����
	return nullptr;
}


void Model::SetNodePoses(const std::vector<NodePose>& nodePoses)
{
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		const NodePose& pose = nodePoses.at(nodeIndex);
		Node& node = nodes.at(nodeIndex);

		node.translate = pose.position;
		node.rotate = pose.rotation;
		node.scale = pose.scale;
	}
}

void Model::GetNodePoses(std::vector<NodePose>& nodePoses) const
{
	if (nodePoses.size() != nodes.size())
	{
		nodePoses.resize(nodes.size());
	}
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		const Node& node = nodes.at(nodeIndex);
		NodePose& pose = nodePoses.at(nodeIndex);

		pose.position = node.translate;
		pose.rotation = node.rotate;
		pose.scale = node.scale;
	}
}

void Model::ComputeAnimation(int animationIndex, int nodeIndex, float time, NodePose& nodePose) const
{
	const Animation& animation = animations.at(animationIndex);
	const NodeAnim& nodeAnim = animation.nodeAnims.at(nodeIndex);

	// �ʒu
	for (size_t index = 0; index < nodeAnim.positionKeyframes.size() - 1; ++index)
	{
		// ���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		const VectorKeyframe& keyframe0 = nodeAnim.positionKeyframes.at(index);
		const VectorKeyframe& keyframe1 = nodeAnim.positionKeyframes.at(index + 1);
		if (time >= keyframe0.seconds && time <= keyframe1.seconds)
		{
			// �Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
			float rate = (time - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			// �O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
			DirectX::XMVECTOR V0 = DirectX::XMLoadFloat3(&keyframe0.value);
			DirectX::XMVECTOR V1 = DirectX::XMLoadFloat3(&keyframe1.value);
			DirectX::XMVECTOR V = DirectX::XMVectorLerp(V0, V1, rate);
			// �v�Z���ʂ��m�[�h�Ɋi�[
			DirectX::XMStoreFloat3(&nodePose.position, V);
		}
	}
	// ��]
	for (size_t index = 0; index < nodeAnim.rotationKeyframes.size() - 1; ++index)
	{
		// ���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		const QuaternionKeyframe& keyframe0 = nodeAnim.rotationKeyframes.at(index);
		const QuaternionKeyframe& keyframe1 = nodeAnim.rotationKeyframes.at(index + 1);
		if (time >= keyframe0.seconds && time <= keyframe1.seconds)
		{
			// �Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
			float rate = (time - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			// �O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
			DirectX::XMVECTOR Q0 = DirectX::XMLoadFloat4(&keyframe0.value);
			DirectX::XMVECTOR Q1 = DirectX::XMLoadFloat4(&keyframe1.value);
			DirectX::XMVECTOR Q = DirectX::XMQuaternionSlerp(Q0, Q1, rate);
			// �v�Z���ʂ��m�[�h�Ɋi�[
			DirectX::XMStoreFloat4(&nodePose.rotation, Q);
		}
	}
	// �X�P�[��
	for (size_t index = 0; index < nodeAnim.scaleKeyframes.size() - 1; ++index)
	{
		// ���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		const VectorKeyframe& keyframe0 = nodeAnim.scaleKeyframes.at(index);
		const VectorKeyframe& keyframe1 = nodeAnim.scaleKeyframes.at(index + 1);
		if (time >= keyframe0.seconds && time <= keyframe1.seconds)
		{
			// �Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
			float rate = (time - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			// �O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
			DirectX::XMVECTOR V0 = DirectX::XMLoadFloat3(&keyframe0.value);
			DirectX::XMVECTOR V1 = DirectX::XMLoadFloat3(&keyframe1.value);
			DirectX::XMVECTOR V = DirectX::XMVectorLerp(V0, V1, rate);
			// �v�Z���ʂ��m�[�h�Ɋi�[
			DirectX::XMStoreFloat3(&nodePose.scale, V);
		}
	}

}

void Model::ComputeAnimation(int animationIndex, float time, std::vector<NodePose>& nodePoses) const
{
	if (nodePoses.size() != nodes.size())
	{
		nodePoses.resize(nodes.size());
	}
	for (size_t nodeIndex = 0; nodeIndex < nodePoses.size(); ++nodeIndex)
	{
		ComputeAnimation(animationIndex, static_cast<int>(nodeIndex), time, nodePoses.at(nodeIndex));
	}

}
