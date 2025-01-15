//
//  AssetManager.cpp
//  SaplingEngine, Seedbank Asset Manager
//

#include "AssetManager.hpp"

AssetManager* AssetManager::Instance = nullptr;

void AssetManager::addTexture(const std::string& name, const std::string& path, const glm::i32 numFrames) {
    auto tex = std::make_shared<Sprout::Texture>();
    
    if (!tex->loadFromFile(ASSETS_PATH + path, numFrames)) 
    {
        throw std::runtime_error("Error loading texture file: " + path);
    }
    getInstance()->m_textures[name] = tex;
}

auto AssetManager::getTexture(const std::string& name) -> std::shared_ptr<Sprout::Texture> {
    auto it = getInstance()->m_textures.find(name);
    if (it == getInstance()->m_textures.end()) {
        throw std::runtime_error("Texture not found: " + name);
    }
    return it->second;  // Return shared_ptr
}