#pragma once
#include <wrl.h>
#include <d3d11.h>



HRESULT load_texture_from_file(ID3D11Device* device, const wchar_t* filename, ID3D11ShaderResourceView** shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc);
void release_all_textures();


// �e�N�X�`��
class Texture
{
public:
    Texture(const char* filename);
    ~Texture() {}

    // �V�F�[�_�[���\�[�X�r���[�擾
    inline const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const { return shaderResourceView; }

    // �e�N�X�`���̏��擾
    inline const D3D11_TEXTURE2D_DESC& GetTexture2dDesc() const { return texture2dDesc; }

    // �e�N�X�`�����擾
    inline int GetWidth() const { return texture2dDesc.Width; }

    // �e�N�X�`�������擾
    inline int GetHeight() const { return texture2dDesc.Height; }



private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;
    D3D11_TEXTURE2D_DESC	texture2dDesc;
};
