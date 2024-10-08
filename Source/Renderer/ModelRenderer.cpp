#include "ModelRenderer.h"

//	�`�惂�f���̓o�^
void ModelRenderer::RegisterRenderModel(Model* model)
{
	// ���f�������ɓo�^����Ă��邩���ׂ�
	std::vector<Model*>::iterator it = std::find(renderModels.begin(), renderModels.end(), model);
	if (it != renderModels.end())
		return;

	renderModels.push_back(model);
}

//	�`�惂�f���̓o�^����
void ModelRenderer::UnregisterRenderModel(Model* model)
{
	// ���f�������ɓo�^����Ă��邩���ׂ�
	std::vector<Model*>::iterator it = std::find(renderModels.begin(), renderModels.end(), model);
	if (it == renderModels.end())
		return;

	renderModels.erase(it);
}

//	�`�惂�f���o�^�S����
void ModelRenderer::ClearRenderModel()
{
	renderModels.clear();
}
