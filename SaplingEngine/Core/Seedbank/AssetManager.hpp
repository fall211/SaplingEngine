//
//  AssetManager.hpp
//  SaplingEngine, Seedbank Asset Manager
//

#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "Renderer/Texture.hpp"
#include "glm/glm.hpp"

#ifndef ASSETS_PATH
#define ASSETS_PATH "../GameContent/Assets/"
#endif

class AssetManager {
    std::unordered_map<std::string, std::shared_ptr<Sprout::Texture>> m_textures = std::unordered_map<std::string, std::shared_ptr<Sprout::Texture>>();
    
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
};

