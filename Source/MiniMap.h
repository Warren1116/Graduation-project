#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <string>
#include "Graphics\Sprite.h"
#include <vector>
#include <memory>
#include <DirectXMath.h>

class MiniMap
{
public:
    static MiniMap& Instance();

    void Initialize(ID3D11Device* device, int size);
    void Update(float elapsedtime);
    void Render(ID3D11DeviceContext* dc);

private:
    void CreateBaseTexture(ID3D11Device* device, int size);
    void CreateTexture(ID3D11Device* device, int size);
    void GenerateBaseMap();
    void UpdateTextureData(ID3D11DeviceContext* dc);

    int textureSize = 256;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> baseTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> baseSRV;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;

    std::vector<UINT> baseMapPixels;
    std::vector<UINT> dynamicPixels;

    std::unique_ptr<Sprite> mapSprite;

    DirectX::XMFLOAT3 stageMin;
    DirectX::XMFLOAT3 stageMax;
};