#include "ResourceManager.h"
#include "Graphics\Graphics.h"

std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const char* filename)
{   
    //モデルを検索
    auto it = models.find(filename);
    if (it != models.end())
    {
        std::shared_ptr<ModelResource> model = it->second.lock();
        if (model) 
        {
            return model;
        }
    }
    //新規モデルリソース作成＆読み込み
    std::shared_ptr<ModelResource> model = std::make_shared<ModelResource>();
    model->Load(Graphics::Instance().GetDevice(), filename);

    //マップに登録
    models[filename] = model;
    return model;

}
