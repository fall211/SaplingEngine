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
    std::unordered_map<std::string, std::shared_ptr<Sprout::Texture>> m_textures;

public:
    AssetManager();

    /*
        * Adds a texture to the asset manager
        * @param name The name of the texture
        * @param path The path to the texture
        * @param numFrames The number of frames in the texture (if animated)
    */
    void addTexture(const std::string& name, 
        const std::string& path,
        glm::i32 numFrames = 1
    );
    
    /*
        * Gets the texture with the given name
        * @param name The name of the texture
        * @return The texture with the given name
    */
    auto getTexture(const std::string& name) -> std::shared_ptr<Sprout::Texture>;
};


