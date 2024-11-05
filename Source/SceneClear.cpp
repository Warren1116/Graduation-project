#include "SceneClear.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "StageMain.h"

void SceneClear::Initialize()
{
}

void SceneClear::Finalize()
{
}

void SceneClear::Update(float elapsedTime)
{

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



}
