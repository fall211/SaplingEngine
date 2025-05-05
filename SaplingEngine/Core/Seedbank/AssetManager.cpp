//
//  AssetManager.cpp
//  SaplingEngine, Seedbank Asset Manager
//

#include "AssetManager.hpp"
#include "Audio/AudioEngine.hpp"
#include "Font.hpp"
#include "Debug.hpp"
#include <filesystem>
#include <iostream>
#include <string>

AssetManager* AssetManager::Instance = nullptr;

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

std::string AssetManager::getAssetsPath() {
    
#ifdef ASSETS_PATH
    return ASSETS_PATH;
#endif
    
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    if (mainBundle) {
        CFURLRef resourceURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
        if (resourceURL) {
            char path[PATH_MAX];
            if (CFURLGetFileSystemRepresentation(resourceURL, TRUE, (UInt8*)path, PATH_MAX)) {
                CFRelease(resourceURL);
                return std::string(path) + "/Assets/";
            }
            CFRelease(resourceURL);
        }
    }
#endif

#ifdef _WIN32
    char path[MAX_PATH];
    DWORD length = GetModuleFileNameA(NULL, path, MAX_PATH);
    if (length > 0 && length < MAX_PATH) {
        std::string fullPath(path);
        size_t pos = fullPath.find_last_of("\\/");
        if (pos != std::string::npos) {
            return fullPath.substr(0, pos) + "\\Assets\\";
        }
    }
#endif

    return "../../GameContent/Assets/"; // fallback path for development
}

void AssetManager::addTexture(const std::string& name, const std::string& path, const glm::i32 numFrames) {
    auto tex = std::make_shared<Sprout::Texture>();
    
    if (!tex->loadFromFile(getAssetsPath() + path, numFrames)) 
    {        
        std::string fullPath = getAssetsPath() + path;
        throw std::runtime_error("Error loading texture file: " + fullPath);
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
    std::vector<std::shared_ptr<Sprout::Texture>> tileset = Sprout::Texture::loadTileset(getAssetsPath() + path, w, h);
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
    FMOD_RESULT result = system->createSound((getAssetsPath() + path).c_str(), mode, nullptr, &sound);
    if (result != FMOD_OK)
    {
        throw std::runtime_error("Error loading sound file: " + path);
    }
    sound->setMode(FMOD_LOOP_NORMAL);
    
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

void AssetManager::addFont(const std::string &name, const std::string &path, float size)
{
    auto font = std::make_shared<Sprout::Font>();
    if (!font->loadFromFile(getAssetsPath() + path, size))
    {
        throw std::runtime_error("Error loading font file: " + path);
    }
    getInstance()->m_fonts[name] = font;
}

auto AssetManager::getFont(const std::string &name) -> std::shared_ptr<Sprout::Font>
{
    auto it = getInstance()->m_fonts.find(name);
    if (it == getInstance()->m_fonts.end()) {
        throw std::runtime_error("Font not found: " + name);
    }
    return it->second;
}
