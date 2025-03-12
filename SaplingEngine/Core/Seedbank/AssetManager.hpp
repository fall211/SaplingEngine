//
//  AssetManager.hpp
//  SaplingEngine, Seedbank Asset Manager
//

#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <memory>
#include <stdexcept>

#include "Renderer/Texture.hpp"
#include "glm/glm.hpp"
#include "fmod.hpp"

class AudioEngine;

#ifndef ASSETS_PATH
#define ASSETS_PATH "../GameContent/Assets/"
#endif

class AssetManager {
    std::unordered_map<std::string, std::shared_ptr<Sprout::Texture>> m_textures = std::unordered_map<std::string, std::shared_ptr<Sprout::Texture>>();
    
    std::unordered_map<std::string, std::vector<std::shared_ptr<Sprout::Texture>>> m_tilesets = std::unordered_map<std::string, std::vector<std::shared_ptr<Sprout::Texture>>>();
    
    std::unordered_map<std::string, FMOD::Sound*> m_sounds = std::unordered_map<std::string, FMOD::Sound*>();
    
    static AssetManager* Instance;

    AssetManager() = default;
    ~AssetManager() = default;

public:


    static AssetManager* getInstance()
    {
        if (Instance == nullptr) {
            Instance = new AssetManager();
        }
        return Instance;
    }

    /*
        * Adds a texture to the asset manager
        * @param name The name of the texture
        * @param path The path to the texture
        * @param numFrames The number of frames in the texture (if animated)
    */
    static void addTexture(const std::string& name, 
        const std::string& path,
        glm::i32 numFrames = 1
    );

    /*
        * Gets the texture with the given name
        * @param name The name of the texture
        * @return The texture with the given name
    */
    static auto getTexture(const std::string& name) -> std::shared_ptr<Sprout::Texture>;
    
    /*
        * Adds a tileset to the asset manager
        * @param name The name of the tileset
        * @param path The path to the tileset
        * @param w The width of the tileset
        * @param h The height of the tileset
    */
    static void addTileSet(const std::string& name, const std::string& path, size_t w, size_t h);
    
    /*
        * Gets the tileset with the given name
        * @param name The name of the tileset
        * @return Pointer to the tileset with the given name
    */
    static auto getTileSet(const std::string& name) -> std::vector<std::shared_ptr<Sprout::Texture>>&;
    
    /*
        * Adds a sound to the asset manager
        * @param name The name of the sound
        * @param path The path to the sound
    */
    static void addSound(const std::string& name, const std::string& path, bool loop = false);
    
    /*
        * Gets the sound with the given name
        * @param name The name of the sound
        * @return Pointer to the sound with the given name
    */
    static auto getSound(const std::string& name) -> FMOD::Sound*;
};

