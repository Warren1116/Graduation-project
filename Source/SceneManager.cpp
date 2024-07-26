#include "SceneManager.h"

void SceneManager::Update(float elapsedTime)
{
    if (nextScene != nullptr)
    {
        //�Â��V�[�����I������
        Clear();

        //if (currentScene != nullptr)
        //{
        //    currentScene->Finalize();
        //    delete currentScene;
        //}

        //�V�����V�[����ݒ�
        currentScene = nextScene;
        nextScene = nullptr;


        ////�V�[������������
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
