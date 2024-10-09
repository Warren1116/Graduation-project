#include "SceneClear.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "StageMain.h"

void SceneClear::Initialize()
{
	clear = std::make_unique<Sprite>("Data/Sprite/clear.png");
	black = std::make_unique<Sprite>("Data/Sprite/black.png");
	text = std::make_unique<Sprite>("Data/Font/font1.png");

	heri = Audio::Instance().LoadAudioSource("Data/Audio/heri2.wav");
	heri->Play(true);
}

void SceneClear::Finalize()
{
}

void SceneClear::Update(float elapsedTime)
{
	if (blackAlpha > 0.0f) blackAlpha -= 0.015f;

	Mouse& mouse = Input::Instance().GetMouse();
	if (blackAlpha <= 0.0f && mouse.GetButtonDown() & (Mouse::BTN_LEFT | Mouse::BTN_MIDDLE | Mouse::BTN_RIGHT))
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}
}

void SceneClear::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 2D
	{	
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = static_cast<float>(clear->GetTextureWidth());
		float textureHeight = static_cast<float>(clear->GetTextureHeight());

		// スプライト描画
		clear->Render(dc,
			0, 0, screenWidth, screenHeight,
			0, 0, textureWidth, textureHeight,
			0,
			1, 1, 1, 1);

		black->Render(dc,
			0, 0, screenWidth, screenHeight,
			0, 0, textureWidth, textureHeight,
			0,
			1, 1, 1, blackAlpha);
		
		if(blackAlpha <= 0.0f && tenmetu / 30 % 2 == 0) text->textout(dc, "Press any Click", 535, 600, 30, 30, { 1, 1, 1, 1 });

		tenmetu++;
	}
}
