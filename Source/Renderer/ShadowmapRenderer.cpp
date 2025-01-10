//#include "ShadowmapRenderer.h"
//#include "Graphics/LightManager.h"
//#include "SceneGame.h"
//#include "Camera.h"
//
//ShadowmapRenderer::ShadowmapRenderer(UINT shadowmapSize)
//{
//    shadowmapDepthStencil.release();
//    shadowmapDepthStencil = std::make_unique<DepthStencil>(shadowmapSize, shadowmapSize);
//
//}
//
//ShadowmapRenderer::~ShadowmapRenderer()
//{
//}
//
//ShadowMapData ShadowmapRenderer::GetShadowMapData()
//{
//    ShadowMapData shadowMapData;
//
//    DirectX::XMFLOAT4X4 view, projection;
//    CalcShadowmapMatrix(view, projection);
//    shadowMapData.shadowMap = shadowmapDepthStencil->GetShaderResourceView().Get();
//    DirectX::XMStoreFloat4x4(&shadowMapData.lightViewProjection,
//        DirectX::XMLoadFloat4x4(&view) * DirectX::XMLoadFloat4x4(&projection));
//    shadowMapData.shadowBias = shadowBias;
//    shadowMapData.shadowColor = shadowColor;
//
//
//    return shadowMapData;
//}
//
//void ShadowmapRenderer::Render(ID3D11DeviceContext* dc)
//{
//    Graphics& graphics = Graphics::Instance();
//
//    // 現在設定されているバッファを退避
//    CacheRenderTargets(dc);
//
//    // 画面クリア＆レンダーターゲット設定
//    ID3D11DepthStencilView* dsv = shadowmapDepthStencil->GetDepthStencilView().Get();
//    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
//    dc->OMSetRenderTargets(0, nullptr, dsv);
//    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//
//    // ビューポートの設定
//    D3D11_VIEWPORT	vp = {};
//    vp.Width = static_cast<float>(shadowmapDepthStencil->GetWidth());
//    vp.Height = static_cast<float>(shadowmapDepthStencil->GetHeight());
//    vp.MinDepth = 0.0f;
//    vp.MaxDepth = 1.0f;
//    dc->RSSetViewports(1, &vp);
//
//    // 描画コンテキストに情報をセット
//    RenderContext rc;
//    {
//        rc.deviceContext = dc;
//        rc.screenSize.x = vp.Width;
//        rc.screenSize.y = vp.Height;
//        rc.screenSize.z = Near;
//        rc.screenSize.w = Far;
//        CalcShadowmapMatrix(rc.view, rc.projection);
//    }
//
//    // 登録モデルを描画
//    ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
//    shader->Begin(rc);
//    for (const Model* model : renderModels)
//    {
//        if (!model) continue;
//        const ModelResource* resource = model->GetResource();
//        const std::vector<Model::Node>& nodes = model->GetNodes();
//        for (const ModelResource::Mesh& mesh : resource->GetMeshes())
//        {
//            //	頂点・インデックスバッファ等設定
//            shader->SetBuffers(rc, nodes, mesh);
//
//            //	サブセット単位で描画
//            for (const ModelResource::Subset& subset : mesh.subsets)
//            {
//                shader->DrawSubset(rc, subset);
//            }
//        }
//    }
//    shader->End(rc);
//
//
//    //	元のバッファに戻す
//    RestoreRenderTargets(dc);
//
//}
//
//
////	デバッグGUI描画
//void ShadowmapRenderer::DrawDebugGUI()
//{
//    if (ImGui::TreeNode("Shadowmap Rendering"))
//    {
//        ImGui::SliderFloat("Rectangle", &shadowRect, 1.0f, 500.0f);
//        ImGui::ColorEdit3("Color", &shadowColor.x);
//        ImGui::SliderFloat("Bias", &shadowBias, 0.0f, 0.1f, "%0.4f");
//
//        if (ImGui::TreeNode("RenderTargets"))
//        {
//            ImGui::Text("Shadowmap");
//
//            ImGui::Image(shadowmapDepthStencil->GetShaderResourceView().Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
//
//            ImGui::TreePop();
//        }
//        ImGui::TreePop();
//    }
//}
//
////	RenderTargetのShaderResourceViewを取得
//const SRVHandleList ShadowmapRenderer::GetRenderTargetShaderResourceViews() const
//{
//    return	SRVHandleList();
//}
//
////	DepthStencilのShaderResourceViewを取得
//const SRVHandle ShadowmapRenderer::GetDepthStencilShaderResourceView() const
//{
//    return	shadowmapDepthStencil->GetShaderResourceView();
//
//}
//
////	シャドウマップで使用する各行列を算出
//void ShadowmapRenderer::CalcShadowmapMatrix(DirectX::XMFLOAT4X4& view, DirectX::XMFLOAT4X4& projection)
//{
//    Light* light = LightManager::Instance().GetShadowmapLight(); // ディレクショナルライトを取得
//    if (!light) // ライトが存在しない場合
//    {
//        view = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
//        projection = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
//        return;
//    }
//
//    // 平行光源からカメラ位置を作成し、そこから原点の位置を見るように視線行列を生成
//    DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&light->GetDirection()); // 方向ベクトル取得
//    LightPosition = DirectX::XMVectorScale(LightPosition, -250.0f); // 取得した方向に対して、伸ばす
//
//    // シャドウマップに描画したい範囲の射影行列を生成
//    DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
//        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
//        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
//    DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowRect, shadowRect, Near, Far);
//    // 生成した値をviewとprojectionに保存
//    DirectX::XMStoreFloat4x4(&view, V);
//    DirectX::XMStoreFloat4x4(&projection, P);
//
//}
