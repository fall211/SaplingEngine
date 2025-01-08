//
//  Assets.hpp
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

class Assets {
    std::unordered_map<std::string, std::shared_ptr<Sprout::Texture>> m_textures;

public:
    Assets();

    void addTexture(const std::string& name, 
        const std::string& path,
        glm::i32 numFrames = 1
    );
    auto getTexture(const std::string& name) -> std::shared_ptr<Sprout::Texture>;
};


