#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Graphics/RenderContext.h"
#include "Graphics/Model.h"
#include "Sprite.h"


class ModelShader
{
public:
	ModelShader() {}
	virtual ~ModelShader() {}

	// �`��J�n
	virtual void Begin(const RenderContext& rc) = 0;

	// �K�v�ȃo�b�t�@�̐ݒ�
	virtual void SetBuffers(const RenderContext& rc, const std::vector<Model::Node>& nodes, const ModelResource::Mesh& mesh) = 0;

	// �T�u�Z�b�g�P�ʂŕ`��
	virtual void DrawSubset(const RenderContext& rc, const ModelResource::Subset& subset) = 0;


	// �`��
	virtual void Draw(const RenderContext& rc, const Model* model) {}

	// �`��I��
	virtual void End(const RenderContext& rc) = 0;
};

class SpriteShader
{
public:
	SpriteShader() {}
	virtual ~SpriteShader() {}

	// �`��J�n
	virtual void Begin(const RenderContext& rc) = 0;

	// �`��
	virtual void Draw(const RenderContext& rc, const Sprite* sprite) = 0;

	// �`��I��
	virtual void End(const RenderContext& rc) = 0;
};
