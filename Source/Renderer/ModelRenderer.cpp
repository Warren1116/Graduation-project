#include "ModelRenderer.h"

//	•`‰æƒ‚ƒfƒ‹‚Ì“o˜^
void ModelRenderer::RegisterRenderModel(Model* model)
{
	renderModels.insert(model);
}

//	•`‰æƒ‚ƒfƒ‹‚Ì“o˜^‰ğœ
void ModelRenderer::UnregisterRenderModel(Model* model)
{
	renderModels.erase(model);
}

//	•`‰æƒ‚ƒfƒ‹“o˜^‘S‰ğœ
void ModelRenderer::ClearRenderModel()
{
	renderModels.clear();
}
