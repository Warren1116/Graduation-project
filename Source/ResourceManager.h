#pragma once

#include <memory>
#include <string>
#include <map>
#include "Graphics\ModelResource.h"

class ResourceManager
{
private:
    ResourceManager() {}
    ~ResourceManager() {}
public:
    static ResourceManager& Instance()
    {
        static ResourceManager instance;
        return instance;
    }

    std::shared_ptr<ModelResource> LoadModelResource(const char* filename);
private:
    using ModelMap = std::map<std::string, std::weak_ptr<ModelResource>>;

    ModelMap models;
};