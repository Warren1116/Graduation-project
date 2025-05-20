#include "MiniMap.h"
#include "Player.h"
#include "EnemyManager.h"
#include "StageManager.h"


using namespace DirectX;

MiniMap& MiniMap::Instance()
{
    static MiniMap instance;
    return instance;
}

void MiniMap::Initialize(ID3D11Device* device, int size)
{
    textureSize = size;
    CreateBaseTexture(device, size);
    GenerateBaseMap();
    CreateTexture(device, size);

    mapSprite = std::make_unique<Sprite>();
    mapSprite->SetShaderResourceView(shaderResourceView.Get(), size, size);
}

void MiniMap::CreateBaseTexture(ID3D11Device* device, int size)
{
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = size;
    desc.Height = size;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    baseMapPixels.resize(size * size, 0xFF000000);

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = baseMapPixels.data();
    initData.SysMemPitch = sizeof(UINT) * size;

    device->CreateTexture2D(&desc, &initData, baseTexture.GetAddressOf());

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    device->CreateShaderResourceView(baseTexture.Get(), &srvDesc, baseSRV.GetAddressOf());
}

void MiniMap::GenerateBaseMap()
{
    for (int y = 0; y < textureSize; ++y)
    {
        for (int x = 0; x < textureSize; ++x)
        {
            float worldX = (x - textureSize / 2.0f);
            float worldZ = (y - textureSize / 2.0f);

            XMVECTOR origin = XMVectorSet(worldX, 100.0f, worldZ, 1.0f);
            XMVECTOR dir = XMVectorSet(0, -1, 0, 0);

            XMFLOAT3 o, d;
            XMStoreFloat3(&o, origin);
            XMStoreFloat3(&d, dir);

            HitResult hit;
            if (StageManager::Instance().RayCast(o, d, hit))
            {
                if (hit.position.y > 5.0f)
                {
                    baseMapPixels[y * textureSize + x] = 0xFF404040;
                }
                else
                {
                    baseMapPixels[y * textureSize + x] = 0xFF000000;
                }
            }
        }
    }
}

void MiniMap::CreateTexture(ID3D11Device* device, int size)
{
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = size;
    desc.Height = size;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    device->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    device->CreateShaderResourceView(texture.Get(), &srvDesc, shaderResourceView.GetAddressOf());
}

void MiniMap::Update(float elapsedtime)
{
    ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
    dynamicPixels = baseMapPixels;

    auto DrawPoint = [&](const XMFLOAT3& pos, UINT color)
        {
            int x = static_cast<int>(pos.x + textureSize / 2.0f);
            int y = static_cast<int>(pos.z + textureSize / 2.0f);
            if (x >= 0 && x < textureSize && y >= 0 && y < textureSize)
            {
                dynamicPixels[y * textureSize + x] = color;
            }
        };

    DrawPoint(Player::Instance().GetPosition(), 0xFF00FF00);

    //for (auto& enemy : EnemyManager::Instance().GetEnemies())
    //{
    //    DrawPoint(enemy->GetPosition(), 0xFFFF0000);
    //}

    UpdateTextureData(dc);
}

void MiniMap::UpdateTextureData(ID3D11DeviceContext* dc)
{
    D3D11_MAPPED_SUBRESOURCE mapped = {};
    if (SUCCEEDED(dc->Map(texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
    {
        UINT* pixels = reinterpret_cast<UINT*>(mapped.pData);
        int pitch = mapped.RowPitch / sizeof(UINT);

        for (int y = 0; y < textureSize; ++y)
        {
            for (int x = 0; x < textureSize; ++x)
            {
                pixels[y * pitch + x] = dynamicPixels[y * textureSize + x];
            }
        }

        dc->Unmap(texture.Get(), 0);
    }
}

void MiniMap::Render(ID3D11DeviceContext* dc)
{
    float screenWidth = Graphics::Instance().GetScreenWidth();
    float screenHeight = Graphics::Instance().GetScreenHeight();

    float mapSize = 200.0f;
    float mapX = screenWidth - mapSize - 50.0f;
    float mapY = 50.0f;

    if (mapSprite)
    {
        mapSprite->Render(dc,
            mapX, mapY,
            mapSize, mapSize,
            0, 0,
            (float)textureSize, (float)textureSize,
            0,
            1, 1, 1, 1);
    }
}
