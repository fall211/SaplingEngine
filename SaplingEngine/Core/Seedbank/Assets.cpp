//
//  Assets.cpp
//  SaplingEngine, Seedbank Asset Manager
//

#include "Assets.hpp"


Assets::Assets() {
    m_textures = std::unordered_map<std::string, std::shared_ptr<Sprout::Texture>>();

}

void Assets::addTexture(const std::string& name, const std::string& path) {
    auto tex = std::make_shared<Sprout::Texture>();
    
    if (!tex->loadFromFile(ASSETS_PATH + path)) 
    {
        throw std::runtime_error("Error loading texture file: " + path);
    }
    m_textures[name] = tex;
}

auto Assets::getTexture(const std::string& name) -> std::shared_ptr<Sprout::Texture> {
    auto it = m_textures.find(name);
    if (it == m_textures.end()) {
        throw std::runtime_error("Texture not found: " + name);
    }
    return it->second;  // Return shared_ptr
}