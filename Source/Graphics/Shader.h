#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Graphics/RenderContext.h"
#include "Graphics/Model.h"
#include "Sprite.h"
#include <cstdio>
#include "Misc.h"
#include <memory>
#include <wrl.h>

using namespace std;


class ModelShader
{
public:
	ModelShader() {}
	virtual ~ModelShader() {}

	// 描画開始
	virtual void Begin(const RenderContext& rc) = 0;

	// 必要なバッファの設定
	virtual void SetBuffers(const RenderContext& rc, const std::vector<Model::Node>& nodes, const ModelResource::Mesh& mesh) = 0;

	// サブセット単位で描画
	virtual void DrawSubset(const RenderContext& rc, const ModelResource::Subset& subset) = 0;

	// 描画
	virtual void Draw(const RenderContext& rc, const Model* model) {}

	// 描画終了
	virtual void End(const RenderContext& rc) = 0;
};

class SpriteShader
{
public:
	SpriteShader() {}
	virtual ~SpriteShader() {}

	// 描画開始
	virtual void Begin(const RenderContext& rc) = 0;

	// 描画
	virtual void Draw(const RenderContext& rc, const Sprite* sprite) = 0;

	// 描画終了
	virtual void End(const RenderContext& rc) = 0;
};


HRESULT create_vs_from_cso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader,
	ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements);

HRESULT create_ps_from_cso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader);

HRESULT create_gs_from_cso(ID3D11Device* device, const char* cso_name, ID3D11GeometryShader** geometry_shader);
