#include "MiniMap.h"
#include "Player.h"
#include "EnemyManager.h"
#include "StageManager.h"
#include "StageMain.h"

using namespace DirectX;

MiniMap& MiniMap::Instance()
{
    static MiniMap instance;
    return instance;
}

//�~�j�}�b�v�̏�����
void MiniMap::Initialize(ID3D11Device* device, int size)
{
    //�e�N�X�`���T�C�Y��ۑ�
    textureSize = size;

    //�X�f�[�W�̃��[���h���W�̍ŏ��l�ƍő�l���擾
    stageMin = StageMain::Instance().GetVolumeMin();
    stageMax = StageMain::Instance().GetVolumeMax();

    //�~�j�}�b�v�̃x�[�X�e�N�X�`���𐶐�
    CreateBaseTexture(device, size);
    //�X�f�[�W�͐ÓI�Ȃ��߁A�x�[�X�}�b�v�𐶐�
    GenerateBaseMap();
    // ���t���[���X�V�����_�C�i�~�b�N�e�N�X�`�����쐬
    CreateTexture(device, size);

    // �X�v���C�g�̏�������SRV�̐ݒ�
    mapSprite = std::make_unique<Sprite>();
    mapSprite->SetShaderResourceView(shaderResourceView.Get(), size, size);
}

//�ÓI�e�N�X�`�����쐬
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

    // �e�N�X�`���쐬
    device->CreateTexture2D(&desc, &initData, baseTexture.GetAddressOf());

    // ShaderResourceView�̐ݒ�
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    device->CreateShaderResourceView(baseTexture.Get(), &srvDesc, baseSRV.GetAddressOf());
}

// �n�`�\�����擾���A�}�b�v�ɐF��h��
void MiniMap::GenerateBaseMap()
{
    //���C�̍�����ݒ�
    float rayStartHeight = 80;

    for (int y = 0; y < textureSize; ++y)
    {
        for (int x = 0; x < textureSize; ++x)
        {
            // UV���W���v�Z
            float u = static_cast<float>(x) / (textureSize - 1);
            float v = static_cast<float>(y) / (textureSize - 1);

            // �X�e�[�W���W�ɕϊ�
            float worldX = stageMin.x + u * (stageMax.x - stageMin.x);
            float worldZ = stageMin.z + v * (stageMax.z - stageMin.z);

            // ���C�L���X�g���g�p���Ēn�`�̍������擾
            XMFLOAT3 origin(worldX, rayStartHeight, worldZ);
            XMFLOAT3 end(worldX, stageMin.y, worldZ);
            HitResult hit;
            if (StageManager::Instance().RayCast(origin, end, hit))
            {
                if (hit.position.y > 5.0f)
                {
                    //�r���̔���
                    baseMapPixels[y * textureSize + x] = 0xFF606060;
                }
                else
                {
                    //�n�ʂ̔���
                    baseMapPixels[y * textureSize + x] = 0xFF202020;
                }
            }

        }
    }
}

//�e�N�X�`�����쐬
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

//�~�j�}�b�v�̍X�V
void MiniMap::Update(float elapsedtime)
{
    ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();

    //�x�[�X�}�b�v�����ɓ��I�s�N�Z���z���������
    dynamicPixels = baseMapPixels;

    //�����_���ŁA�v���C���[�ƓG�̈ʒu��`��
    auto DrawPoint = [&](const XMFLOAT3& pos, UINT color, int radius = 1)
        {
            //���W�ϊ�
            float u = (pos.x - stageMin.x) / (stageMax.x - stageMin.x);
            float v = (pos.z - stageMin.z) / (stageMax.z - stageMin.z);

            int cx = static_cast<int>(u * textureSize);
            int cy = static_cast<int>(v * textureSize);

            
            for (int dy = -radius; dy <= radius; ++dy)
            {
                for (int dx = -radius; dx <= radius; ++dx)
                {
                    int x = cx + dx;
                    int y = cy + dy;

                    if (x >= 0 && x < textureSize && y >= 0 && y < textureSize)
                    {
                        dynamicPixels[y * textureSize + x] = color;
                    }
                }
            }
        };
    DrawPoint(Player::Instance().GetPosition(), 0xFFFFFFFF);

    float EnemyCount = EnemyManager::Instance().GetEnemyCount();
    for (int i = 0; i < EnemyCount; ++i)
    {
        Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
        if (enemy != nullptr)
        {
            DrawPoint(enemy->GetPosition(), 0xFF0000FF);
        }
    }

    // �e�N�X�`���ɓ��I�s�N�Z���z�����������
    UpdateTextureData(dc);
}

//�e�N�X�`���f�[�^��GPU�ɓ]��
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

//�~�j�}�b�v�̕`��
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
