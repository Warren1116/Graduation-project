#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "SceneManager.h"

void SceneLoading::Initialize()
{
    sprite = new Sprite("Data/Sprite/LoadingIcon.png");
    thread = new std::thread(LoadingThread, this);
}

void SceneLoading::Finalize()
{
    if (thread != nullptr)
    {
        //先生はjoinを使う、下のdetach使っていない
        thread->join();

        delete thread;
        thread = nullptr;
    }
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }
}

void SceneLoading::Update(float elapsedTime)
{
    constexpr float speed = 180;
    angle += speed * elapsedTime;
    if (nextScene->IsReady())
    {
        //先生はdetachを使ってない
        //this->thread->detach();

        SceneManager::Instance().ChangeScene(nextScene);
        nextScene = nullptr;
    }
}


void SceneLoading::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    {
        float screewWidth = static_cast<float>(graphics.GetScreenWidth());
        float screewHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureWidth());
        float positionX = screewWidth - textureWidth;
        float positionY = screewHeight - textureHeight;

        sprite->Render(dc,
            positionX, positionY, textureWidth, textureHeight,
            0, 0, textureWidth, textureHeight,
            angle,
            1, 1, 1, 1);
    }
}

void SceneLoading::LoadingThread(SceneLoading* scene)
{
    CoInitialize(nullptr);

    scene->nextScene->Initialize();

    CoUninitialize();

    scene->nextScene->SetReady();
}
