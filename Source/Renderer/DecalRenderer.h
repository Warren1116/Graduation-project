//#pragma once
//
//#include "Renderer/BaseRenderer.h"
//#include <d3d11.h>
//#include <wrl/client.h>
//#include <DirectXMath.h>
//#include <memory>
//#include <vector>
//#include "Graphics/Graphics.h"
//#include "Graphics/Sprite.h"
//
//class DecalRenderer : public BaseRenderer
//{
//public:
//    enum GBufferId
//    {
//        GB_BaseColor = 0,
//        GB_Emissive,
//        GB_Normal,
//        GB_Parameters,
//        GB_Depth,
//        GB_Velocity,
//        GB_Max,
//    };
//
//    DecalRenderer(UINT width, UINT height);
//    ~DecalRenderer();
//
//    void Initialize();
//    void Render(ID3D11DeviceContext* dc) override;
//    void DrawDebugGUI() override;
//    const SRVHandleList GetRenderTargetShaderResourceViews() const override;
//    const SRVHandle GetDepthStencilShaderResourceView() const override;
//    void AddDecal(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT4& color);
//
//private:
//    struct DecalTexture
//    {
//        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> colorShaderResourceView;
//    };
//
//    struct DecalData
//    {
//        DirectX::XMFLOAT3 translation{ 0, 0, 0 };
//        DirectX::XMFLOAT3 scaling{ 1, 1, 1 };
//        DirectX::XMFLOAT3 rotation{ 0, 0, 0 };
//        DirectX::XMFLOAT4 color{ 1, 1, 1, 1 };
//        int decalIndex = -1;
//    };
//
//    struct GBufferDecalConstants
//    {
//        DirectX::XMFLOAT4X4 decalInverseTransform;
//        DirectX::XMFLOAT4 decalDirection;
//    };
//
//    UINT width;
//    UINT height;
//    std::unique_ptr<Sprite> decalSprite;
//    std::vector<DecalTexture> decalTextures;
//    std::vector<DecalData> decalDatas;
//    Microsoft::WRL::ComPtr<ID3D11Buffer> gbufferDecalConstantBuffer;
//    Microsoft::WRL::ComPtr<ID3D11PixelShader> gbufferDecalPixelShader;
//    Microsoft::WRL::ComPtr<ID3D11PixelShader> gbufferDecalSpritePixelShader;
//    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> decalDepthStencilState;
//
//    // ディファードレンダリング関係
//    Microsoft::WRL::ComPtr<ID3D11BlendState> gBufferBlendState;
//    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> gBufferRenderTargetView[GB_Max];
//    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> gBufferShaderResourceView[GB_Max];
//    Microsoft::WRL::ComPtr<ID3D11PixelShader> deferredRenderingEmissivePixelShader;
//    Microsoft::WRL::ComPtr<ID3D11PixelShader> deferredRenderingIndirectPixelShader;
//    Microsoft::WRL::ComPtr<ID3D11PixelShader> deferredRenderingDirectionalPixelShader;
//    std::unique_ptr<Sprite> deferredRenderingSprite;
//    Microsoft::WRL::ComPtr<ID3D11Buffer> shadowmapSceneConstantBuffer;
//    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> sceneRenderTargetView;
//    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sceneShaderResourceView;
//    Microsoft::WRL::ComPtr<ID3D11PixelShader> finalPassPixelShader;
//    std::unique_ptr<Sprite> finalPassRenderSprite;
//};