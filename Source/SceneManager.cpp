#include "SceneManager.h"

void SceneManager::Update(float elapsedTime)
{
    if (nextScene != nullptr)
    {
        //古いシーンを終了処理
        Clear();

        //if (currentScene != nullptr)
        //{
        //    currentScene->Finalize();
        //    delete currentScene;
        //}

        //新しいシーンを設定
        currentScene = nextScene;
        nextScene = nullptr;


        ////シーン初期化処理
        if (!(currentScene->IsReady()))
        {
            currentScene->Initialize();
        }

    }

    if (currentScene != nullptr)
    {
        currentScene->Update(elapsedTime);
    }


}

void SceneManager::Render()
{
    if (currentScene != nullptr)
    {
        currentScene->Render();
    }
}

void SceneManager::Clear()
{
    if (currentScene != nullptr)
    {
        currentScene->Finalize();
        delete currentScene;

        currentScene = nullptr;
    }
}

void SceneManager::ChangeScene(Scene* scene)
{
    nextScene = scene;

}
