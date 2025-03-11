//
//  AssetManager.cpp
//  SaplingEngine, Seedbank Asset Manager
//

#include "AssetManager.hpp"
#include "Audio/AudioEngine.hpp"

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
    
    return it->second;
}

void AssetManager::addTileSet(const std::string& name, const std::string& path, size_t w, size_t h)
{
    std::vector<std::shared_ptr<Sprout::Texture>> tileset = Sprout::Texture::loadTileset(ASSETS_PATH + path, w, h);
    if (tileset.empty())
    {
        throw std::runtime_error("Error loading tileset file: " + path);
    }
    getInstance()->m_tilesets[name] = tileset;
}

auto AssetManager::getTileSet(const std::string &name) -> std::vector<std::shared_ptr<Sprout::Texture>>&
{
    auto it = getInstance()->m_tilesets.find(name);
    if (it == getInstance()->m_tilesets.end()) {
        throw std::runtime_error("Tileset not found: " + name);
    }
    return it->second;
}

void AssetManager::addSound(const std::string &name, const std::string &path, bool loop) 
{
    FMOD::Sound* sound = nullptr;
    FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;
    
    FMOD::System* system = AudioEngine::getInstance()->getSystem();
    FMOD_RESULT result = system->createSound((ASSETS_PATH + path).c_str(), mode, nullptr, &sound);
    if (result != FMOD_OK)
    {
        throw std::runtime_error("Error loading sound file: " + path);
    }
    getInstance()->m_sounds[name] = sound;
}

auto AssetManager::getSound(const std::string &name) -> FMOD::Sound*
{
    auto it = getInstance()->m_sounds.find(name);
    if (it == getInstance()->m_sounds.end()) {
        throw std::runtime_error("Sound not found: " + name);
    }
    return it->second;
}
