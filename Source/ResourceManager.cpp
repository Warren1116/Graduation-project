#include "ResourceManager.h"
#include "Graphics\Graphics.h"

std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const char* filename)
{   
    //���f��������
    auto it = models.find(filename);
    if (it != models.end())
    {
        std::shared_ptr<ModelResource> model = it->second.lock();
        if (model) 
        {
            return model;
        }
    }
    //�V�K���f�����\�[�X�쐬���ǂݍ���
    std::shared_ptr<ModelResource> model = std::make_shared<ModelResource>();
    model->Load(Graphics::Instance().GetDevice(), filename);

    //�}�b�v�ɓo�^
    models[filename] = model;
    return model;

}
