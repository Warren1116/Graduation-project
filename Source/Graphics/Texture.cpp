#include <stdlib.h>
#include "Graphics/Graphics.h"
#include "Graphics/Texture.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <sstream>
#include <filesystem>
#include <map>
using namespace std;

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Misc.h"
#include "Logger.h"

Texture::Texture(const char* filename)
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	// パスを分解
	char drive[256], dirname[256], fname[256], ext[256];
	::_splitpath_s(filename, drive, sizeof(drive), dirname, sizeof(dirname), fname, sizeof(fname), ext, sizeof(ext));

	// パスを結合
	char filepath[256];
	::_makepath_s(filepath, 256, drive, dirname, fname, ext);

	// マルチバイト文字からワイド文字へ変換
	wchar_t wfilepath[256];
	::MultiByteToWideChar(CP_ACP, 0, filepath, -1, wfilepath, 256);

	// テクスチャ読み込み
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	HRESULT hr = DirectX::CreateWICTextureFromFile(device, wfilepath, resource.GetAddressOf(), shaderResourceView.ReleaseAndGetAddressOf());
	// DDSとして読み込んでみる
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(device, wfilepath, resource.GetAddressOf(), shaderResourceView.ReleaseAndGetAddressOf());
	}
	// WICでサポートされていないフォーマットの場合（TGAなど）は
	// STBで画像読み込みをしてテクスチャを生成する
	if (FAILED(hr))
	{
		int width, height, bpp;
		unsigned char* pixels = stbi_load(filepath, &width, &height, &bpp, STBI_rgb_alpha);
		if (pixels != nullptr)
		{
			D3D11_TEXTURE2D_DESC desc = { 0 };
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			D3D11_SUBRESOURCE_DATA data;
			::memset(&data, 0, sizeof(data));
			data.pSysMem = pixels;
			data.SysMemPitch = width * 4;

			Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture2d;
			hr = device->CreateTexture2D(&desc, &data, texture2d.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

			hr = device->CreateShaderResourceView(texture2d.Get(), nullptr, shaderResourceView.ReleaseAndGetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

			// 後始末
			stbi_image_free(pixels);
		}
	}
	if (SUCCEEDED(hr))
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture2d;
		hr = resource.Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		texture2d->GetDesc(&texture2dDesc);
	}
}



//static map<wstring, ComPtr<ID3D11ShaderResourceView>> resources;
//HRESULT load_texture_from_file(ID3D11Device* device, const wchar_t* filename, ID3D11ShaderResourceView** shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc)
//{
//	HRESULT hr{ S_OK };
//	ComPtr<ID3D11Resource> resource;
//
//	auto it = resources.find(filename);
//	if (it != resources.end())
//	{
//		*shader_resource_view = it->second.Get();
//		(*shader_resource_view)->AddRef();
//		(*shader_resource_view)->GetResource(resource.GetAddressOf());
//	}
//	else
//	{
//		// UNIT.31
//		std::filesystem::path dds_filename(filename);
//		dds_filename.replace_extension("dds");
//		if (std::filesystem::exists(dds_filename.c_str()))
//		{
//			hr = DirectX::CreateDDSTextureFromFile(device, dds_filename.c_str(), resource.GetAddressOf(), shader_resource_view);
//			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
//		}
//		else
//		{
//			hr = DirectX::CreateWICTextureFromFile(device, filename, resource.GetAddressOf(), shader_resource_view);
//			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
//		}
//		resources.insert(make_pair(filename, *shader_resource_view));
//	}
//
//	// DECAL
//	if (texture2d_desc)
//	{
//		ComPtr<ID3D11Texture2D> texture2d;
//		hr = resource.Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
//		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
//		texture2d->GetDesc(texture2d_desc);
//	}
//
//	return hr;
//}
//void release_all_textures()
//{
//	resources.clear();
//}
//
