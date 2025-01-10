//#include "ShadowmapCasterRenderer.h"
//#include "Graphics/LightManager.h"
//#include "SceneGame.h"
//
//ShadowmapCasterRenderer::ShadowmapCasterRenderer(UINT shadowmapSize)
//{
//    //for (int i = 0; i < ShadowmapCount; ++i)
//    //{
//    //    shadowmapDepthStencil[ShadowmapCount].release();
//    //    shadowmapDepthStencil[ShadowmapCount] = std::make_unique<DepthStencil>(shadowmapSize, shadowmapSize);
//    //}
//
//    for (auto& it : shadowmapDepthStencil)
//    {
//        it.release();
//        it = std::make_unique<DepthStencil>(shadowmapSize, shadowmapSize);
//    }
//}
//
//ShadowmapCasterRenderer::~ShadowmapCasterRenderer()
//{
//}
//
//ShadowMapData ShadowmapCasterRenderer::GetShadowMapData()
//{
//    ShadowMapData shadowMapData;
//
//    for (int i = 0; i < ShadowmapCount; ++i)
//    {
//        shadowMapData.shadowMap[i] = shadowmapDepthStencil[i]->GetShaderResourceView().Get();
//        DirectX::XMFLOAT4X4 view, projection;
//        CalcShadowmapMatrix(view, projection,i);
//        DirectX::XMStoreFloat4x4(shadowMapData.lightViewProjection,
//            DirectX::XMLoadFloat4x4(&view) * DirectX::XMLoadFloat4x4(&projection));
//        shadowMapData.shadowBias[i] = shadowBias[i];
//    }
//    shadowMapData.shadowColor = shadowColor;
//    return shadowMapData;
//}
//
//void ShadowmapCasterRenderer::Render(ID3D11DeviceContext* dc)
//{
//    Graphics& graphics = Graphics::Instance();
//
//    // ���ݐݒ肳��Ă���o�b�t�@��ޔ�
//    CacheRenderTargets(dc);
//
//    for (int i = 0; i < ShadowmapCount; ++i)
//    {
//        // ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
//        ID3D11DepthStencilView* dsv = shadowmapDepthStencil[i]->GetDepthStencilView().Get();
//        FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
//        dc->OMSetRenderTargets(0, nullptr, dsv);
//        dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//        // �r���[�|�[�g�̐ݒ�
//        D3D11_VIEWPORT	vp = {};
//        vp.Width = static_cast<float>(shadowmapDepthStencil[i]->GetWidth());
//        vp.Height = static_cast<float>(shadowmapDepthStencil[i]->GetHeight());
//        vp.MinDepth = 0.0f;
//        vp.MaxDepth = 1.0f;
//        dc->RSSetViewports(1, &vp);
//
//        // �`��R���e�L�X�g�ɏ����Z�b�g
//        RenderContext rc;
//        {
//            rc.deviceContext = dc;
//            rc.screenSize.x = vp.Width;
//            rc.screenSize.y = vp.Height;
//            rc.screenSize.z = Near;
//            rc.screenSize.w = Far;
//            CalcShadowmapMatrix(rc.view, rc.projection,i);
//        }
//
//
//        // �o�^���f����`��
//        ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
//        shader->Begin(rc);
//        for (const Model* model : renderModels)
//        {
//            if (!model) continue;
//            const ModelResource* resource = model->GetResource();
//            const std::vector<Model::Node>& nodes = model->GetNodes();
//            for (const ModelResource::Mesh& mesh : resource->GetMeshes())
//            {
//                //	���_�E�C���f�b�N�X�o�b�t�@���ݒ�
//                shader->SetBuffers(rc, nodes, mesh);
//
//                //	�T�u�Z�b�g�P�ʂŕ`��
//                for (const ModelResource::Subset& subset : mesh.subsets)
//                {
//                    shader->DrawSubset(rc, subset);
//                }
//            }
//        }
//        shader->End(rc);
//    }
//
//    //	���̃o�b�t�@�ɖ߂�
//    RestoreRenderTargets(dc);
//
//}
//
//
////	�f�o�b�OGUI�`��
//void ShadowmapCasterRenderer::DrawDebugGUI()
//{
//    if (ImGui::TreeNode("Shadowmap Rendering"))
//    {
//        ImGui::SliderFloat("Rectangle", &shadowRect, 1.0f, 2048.0f);
//        ImGui::ColorEdit3("Color", &shadowColor.x);
//        for (int i = 0; i < ShadowmapCount; ++i)
//        {
//            ImGui::SliderFloat((std::string("Bias") + std::to_string(i)).c_str(),
//                &shadowBias[i], 0.0f, 0.1f);
//        }
//
//        if (ImGui::TreeNode("RenderTargets"))
//        {
//            ImGui::Text("Shadowmap");
//
//			for (int i = 0; i < ShadowmapCount; ++i)
//			{
//				ImGui::Image(shadowmapDepthStencil[0]->GetShaderResourceView().Get(), {256, 256}, {0, 0}, {1, 1}, {1, 1, 1, 1} );
//				ImGui::SameLine();
//			}
//
//            ImGui::TreePop();
//        }
//        ImGui::TreePop();
//    }
//}
//
////	RenderTarget��ShaderResourceView���擾
//const SRVHandleList ShadowmapCasterRenderer::GetRenderTargetShaderResourceViews() const
//{
//    return SRVHandleList();
//}
//
//
////	DepthStencil��ShaderResourceView���擾
//const SRVHandle ShadowmapCasterRenderer::GetDepthStencilShaderResourceView() const
//{
//    for (int i = 0; i < ShadowmapCount; ++i)
//    {
//        return	shadowmapDepthStencil[i]->GetShaderResourceView();
//    }
//
//}
//
////	�V���h�E�}�b�v�Ŏg�p����e�s����Z�o
//void ShadowmapCasterRenderer::CalcShadowmapMatrix(DirectX::XMFLOAT4X4& view, DirectX::XMFLOAT4X4& projection)
//{
//    Light* light = LightManager::Instance().GetShadowmapLight();
//    if (!light)
//    {
//        view = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
//        projection = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
//        return;
//    }
//
//    // ���s��������J�����ʒu���쐬���A�������猴�_�̈ʒu������悤�Ɏ����s��𐶐�
//    DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&light->GetDirection());
//    LightPosition = DirectX::XMVectorScale(LightPosition, -250.0f);
//
//    DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
//        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
//        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
//    DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowRect, shadowRect, Near, Far);
//    // �V���h�E�}�b�v�ɕ`�悵�����͈͂̎ˉe�s��𐶐�
//    DirectX::XMStoreFloat4x4(&view, V);
//    DirectX::XMStoreFloat4x4(&projection, P);
//}
//
////	�V���h�E�}�b�v�Ŏg�p����e�s����Z�o
//void ShadowmapCasterRenderer::CalcShadowmapMatrix(DirectX::XMFLOAT4X4& view, DirectX::XMFLOAT4X4& projection, int cascadeIndex)
//{
//
//    Light* light = LightManager::Instance().GetShadowmapLight();
//    if (!light)
//    {
//        view = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
//        projection = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
//        return;
//    }
//
//    // ���s��������J�����ʒu���쐬���A�������猴�_�̈ʒu������悤�Ɏ����s��𐶐�
//    DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&light->GetDirection());
//    LightPosition = DirectX::XMVectorScale(LightPosition, -250.0f);
//
//    // �J�X�P�[�h���Ƃ͈̔͂�ݒ�
//    float cascadeSplits[ShadowmapCount] = { 0.1f, 0.3f, 0.6f, 1.0f };
//    float nearPlane = Near + (Far - Near) * (cascadeIndex == 0 ? 0.0f : cascadeSplits[cascadeIndex - 1]);
//    float farPlane = Near + (Far - Near) * cascadeSplits[cascadeIndex];
//
//    DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
//        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
//        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
//    DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowRect, shadowRect, nearPlane, farPlane);
//    // �V���h�E�}�b�v�ɕ`�悵�����͈͂̎ˉe�s��𐶐�
//    DirectX::XMStoreFloat4x4(&view, V);
//    DirectX::XMStoreFloat4x4(&projection, P);
//}
//
