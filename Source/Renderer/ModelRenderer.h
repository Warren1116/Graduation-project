#pragma once

#include "BaseRenderer.h"
#include "Graphics/Model.h"

class ModelRenderer : public BaseRenderer
{
public:
	ModelRenderer() {}
	virtual ~ModelRenderer() {}

public:
	//	�`�惂�f���̓o�^
	void RegisterRenderModel(Model* model);

	//	�`�惂�f���̓o�^����
	void UnregisterRenderModel(Model* model);

	//	�`�惂�f���o�^�S����
	void ClearRenderModel();

protected:
	std::vector<Model*>	renderModels; // �`�惂�f�����X�g
};
