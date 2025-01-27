#include "ModelRenderer.h"

//	�`�惂�f���̓o�^
void ModelRenderer::RegisterRenderModel(Model* model)
{
	renderModels.insert(model);
}

//	�`�惂�f���̓o�^����
void ModelRenderer::UnregisterRenderModel(Model* model)
{
	renderModels.erase(model);
}

//	�`�惂�f���o�^�S����
void ModelRenderer::ClearRenderModel()
{
	renderModels.clear();
}
