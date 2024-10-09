#include "Graphics/Graphics.h"
#include "ResourceManager.h"

// モデルのリソース読み込み
std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const char* filename)
{
    // モデルを検索
    ModelMap::iterator it = models.find(filename);
    if (it != models.end()) 
    {
        // リンクが切れていないか確認
        if (!it->second.expired())
        {
            // 読み込み済みのモデルリソースを返す
            return it->second.lock();
        }
    }

    // 新規モデルリソース作成＆読み込み
    ID3D11Device* device = Graphics::Instance().GetDevice();
    auto model = std::make_shared<ModelResource>();
    model->Load(device, filename);
    
    // マップに登録
    models[filename] = model;

    return model;
}
