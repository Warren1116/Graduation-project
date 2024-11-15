#pragma once

#include <vector>
#include <memory>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

class Font
{
public:
	Font(const char* filename, int maxSpriteCount);
	~Font() {}

	void Begin(ID3D11DeviceContext* context);
	void Draw(float x, float y, const wchar_t* string, float xScale = 1.0f, float yScale = 1.0f, DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
	void Draw(float x, float y, const std::string& string, float xScale = 1.0f, float yScale = 1.0f, DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f })
	{
		Draw(x, y, StringToWString(string).c_str(), xScale, yScale, color);
	}
	void End(ID3D11DeviceContext* context);

	// 文字列の縦横幅を算出
	DirectX::XMFLOAT2 CalcStringSize(const wchar_t* string, float xScale = 1.0f, float yScale = 1.0f);
	DirectX::XMFLOAT2 CalcStringSize(const std::string& string, float xScale = 1.0f, float yScale = 1.0f)
	{
		return	CalcStringSize(StringToWString(string).c_str(), xScale, yScale);
	}

	// stringをwstringへ変換する
	std::wstring StringToWString(const std::string& string);

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>						vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>						pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>						inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>							vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>							indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11BlendState>						blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>					rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>					depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>						samplerState;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>	shaderResourceViews;

	struct Vertex
	{
		DirectX::XMFLOAT3			position;
		DirectX::XMFLOAT4			color;
		DirectX::XMFLOAT2			texcoord;
	};

	struct CharacterInfo
	{
		static const WORD			NonCode = 0;			// コードなし
		static const WORD			EndCode = 0xFFFF;		// 終了コード
		static const WORD			ReturnCode = 0xFFFE;	// 改行コード
		static const WORD			TabCode = 0xFFFD;		// タブコード
		static const WORD			SpaceCode = 0xFFFC;		// 空白コード

		float						left;					// テクスチャ左座標
		float						top;					// テクスチャ上座標
		float						right;					// テクスチャ右座標
		float						bottom;					// テクスチャ下座標
		float						xoffset;				// オフセットX
		float						yoffset;				// オフセットY
		float						xadvance;				// プロポーショナル幅
		float						width;					// 画像幅
		float						height;					// 画像高さ
		int							page;					// テクスチャ番号
		bool						ascii;					// アスキー文字
	};

	float							fontWidth;
	float							fontHeight;
	int								textureCount;
	int								characterCount;
	std::vector<CharacterInfo>		characterInfos;
	std::vector<WORD>				characterIndices;

	struct Subset
	{
		ID3D11ShaderResourceView* shaderResourceView;
		UINT						startIndex;
		UINT						indexCount;
	};
	std::vector<Subset>				subsets;
	Vertex* currentVertex = nullptr;
	UINT							currentIndexCount;
	int								currentPage;

	float							screenWidth;
	float							screenHeight;
};
