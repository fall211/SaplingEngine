//
//  Assets.cpp
//  SaplingEngine, Seedbank Asset Manager
//

#include "Assets.hpp"


Assets::Assets() {
    m_textures = std::unordered_map<std::string, std::shared_ptr<Sprout::Texture>>();
}

void Assets::addTexture(const std::string& name, const std::string& path) {
    Sprout::Texture tex;
    if (!tex.loadFromFile(ASSETS_PATH + path)) 
    {
        throw std::runtime_error("Error loading texture file: " + path);
    }
    m_textures[name] = std::make_shared<Sprout::Texture>(tex);
}

auto Assets::getTexture(const std::string& name) -> std::shared_ptr<Sprout::Texture> {
    return m_textures[name];
}