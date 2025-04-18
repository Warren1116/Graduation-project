//#include "DecalRenderer.h"
//#include "Graphics/Graphics.h"
//#include "Camera.h"
//
//DecalRenderer::DecalRenderer(UINT width, UINT height)
//    : width(width), height(height)
//{
//}
//
//DecalRenderer::~DecalRenderer()
//{
//}
//
//void DecalRenderer::Initialize()
//{
//    Graphics& graphics = Graphics::Instance();
//    ID3D11Device* device = graphics.GetDevice();
//
//    // �f�J�[���p�̒萔�o�b�t�@���쐬
//    D3D11_BUFFER_DESC bufferDesc = {};
//    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
//    bufferDesc.ByteWidth = sizeof(GBufferDecalConstants);
//    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//    bufferDesc.CPUAccessFlags = 0;
//    HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, gbufferDecalConstantBuffer.GetAddressOf());
//    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
//
//    // �f�J�[���p�̃s�N�Z���V�F�[�_�[�����[�h
//    hr = D3DReadFileToBlob(L"Shaders/GBufferDecalPS.cso", gbufferDecalPixelShader.GetAddressOf());
//    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
//
//    // �s�N�Z���V�F�[�_�[
//    {
//        // �t�@�C�����J��
//        FILE* fp = nullptr;
//        fopen_s(&fp, "Shader\\GBufferDecalPS.cso", "rb");
//        _ASSERT_EXPR_A(fp, "CSO File not found");
//
//        // �t�@�C���̃T�C�Y�����߂�
//        fseek(fp, 0, SEEK_END);
//        long csoSize = ftell(fp);
//        fseek(fp, 0, SEEK_SET);
//
//        // ��������Ƀs�N�Z���V�F�[�_�[�f�[�^���i�[����̈��p�ӂ���
//        std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
//        fread(csoData.get(), csoSize, 1, fp);
//        fclose(fp);
//
//        // �s�N�Z���V�F�[�_�[����
//        HRESULT hr = device->CreatePixelShader(csoData.get(), csoSize, nullptr, gbufferDecalPixelShader.GetAddressOf());
//        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//    }
//
//
//
//    hr = D3DReadFileToBlob(L"Shaders/GBufferDecalSpritePS.cso", gbufferDecalSpritePixelShader.GetAddressOf());
//    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
//
//    // �f�J�[���p�̃X�v���C�g���쐬
//    decalSprite = std::make_unique<Sprite>();
//
//    // �f�B�t�@�[�h�����_�����O�֌W�̏�����
//    // G�o�b�t�@�̃����_�[�^�[�Q�b�g�r���[�ƃV�F�[�_�[���\�[�X�r���[���쐬
//    D3D11_TEXTURE2D_DESC textureDesc = {};
//    textureDesc.Width = width;
//    textureDesc.Height = height;
//    textureDesc.MipLevels = 1;
//    textureDesc.ArraySize = 1;
//    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//    textureDesc.SampleDesc.Count = 1;
//    textureDesc.Usage = D3D11_USAGE_DEFAULT;
//    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
//
//    for (int i = 0; i < GB_Max; ++i)
//    {
//        Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
//        hr = device->CreateTexture2D(&textureDesc, nullptr, texture.GetAddressOf());
//        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
//
//        hr = device->CreateRenderTargetView(texture.Get(), nullptr, gBufferRenderTargetView[i].GetAddressOf());
//        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
//
//        hr = device->CreateShaderResourceView(texture.Get(), nullptr, gBufferShaderResourceView[i].GetAddressOf());
//        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
//    }
//
//    // �u�����h�X�e�[�g�̍쐬
//    D3D11_BLEND_DESC blendDesc = {};
//    blendDesc.RenderTarget[0].BlendEnable = TRUE;
//    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
//    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
//    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
//    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
//    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
//    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
//    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
//    hr = device->CreateBlendState(&blendDesc, gBufferBlendState.GetAddressOf());
//    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
//}
//
//void DecalRenderer::Render(ID3D11DeviceContext* dc)
//{
//    Graphics& graphics = Graphics::Instance();
//    Camera& camera = Camera::Instance();
//
//    // �f�J�[���̕`�揈��
//    for (const auto& decal : decalDatas)
//    {
//        // �萔�o�b�t�@�̍X�V
//        GBufferDecalConstants constants;
//        DirectX::XMStoreFloat4x4(&constants.decalInverseTransform, DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixScaling(decal.scaling.x, decal.scaling.y, decal.scaling.z) * DirectX::XMMatrixRotationRollPitchYaw(decal.rotation.x, decal.rotation.y, decal.rotation.z) * DirectX::XMMatrixTranslation(decal.translation.x, decal.translation.y, decal.translation.z)));
//        constants.decalDirection = DirectX::XMFLOAT4(decal.translation.x, decal.translation.y, decal.translation.z, 1.0f);
//        dc->UpdateSubresource(gbufferDecalConstantBuffer.Get(), 0, nullptr, &constants, 0, 0);
//
//        // �V�F�[�_�[�̐ݒ�
//        dc->PSSetConstantBuffers(0, 1, gbufferDecalConstantBuffer.GetAddressOf());
//        dc->PSSetShader(gbufferDecalPixelShader.Get(), nullptr, 0);
//
//        // �f�J�[���̕`��
//        decalSprite->SetShaderResourceView(decalTextures[decal.decalIndex].colorShaderResourceView, width, height);
//        decalSprite->Update(0, 0, width, height, 0, 0, static_cast<float>(width), static_cast<float>(height), 0, 1, 1, 1, 1);
//        decalSprite->Draw(dc);
//    }
//}
//
//void DecalRenderer::DrawDebugGUI()
//{
//    if (ImGui::TreeNode("DecalRenderer"))
//    {
//        // �f�J�[���̃f�o�b�O����\��
//        ImGui::Text("Number of Decals: %d", static_cast<int>(decalDatas.size()));
//        ImGui::TreePop();
//    }
//}
//
//const SRVHandleList DecalRenderer::GetRenderTargetShaderResourceViews() const
//{
//    return SRVHandleList();
//}
//
//const SRVHandle DecalRenderer::GetDepthStencilShaderResourceView() const
//{
//    return SRVHandle();
//}
//
//void DecalRenderer::AddDecal(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT4& color)
//{
//    DecalData decal;
//    decal.translation = position;
//    decal.scaling = scale;
//    decal.rotation = rotation;
//    decal.color = color;
//    decal.decalIndex = 0; // �f�J�[���e�N�X�`���̃C���f�b�N�X��ݒ�
//    decalDatas.push_back(decal);
//}