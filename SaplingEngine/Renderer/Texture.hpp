//
//  Texture.hpp
//  Sapling Engine, Sprout Renderer
//

#pragma once

#include <string>
#include <glm/glm.hpp>


namespace Sprout {
    
    class Texture {
    public:
        Texture() = default;
        ~Texture();
        
        auto loadFromFile(const std::string& path, glm::i32 numFrames = 1) -> bool;
        auto getPixels() -> unsigned char*;
        auto getSize() -> glm::vec2;
        auto getWidth() -> glm::i32;
        auto getHeight() -> glm::i32;
        auto getAtlasUVs() -> glm::vec4;
        auto setAtlasUVs(glm::vec4 uvs) -> void;
        auto getFrameSize() -> glm::vec2;
        auto getNumFrames() -> glm::i32;
        
        auto registerTexture() -> void;
        
    private:
        glm::i32 m_width, m_height;
        glm::vec4 m_atlas_uvs = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        unsigned char* m_pixels;
        
        // animated sprites data
        glm::i32 m_frameWidth, m_frameHeight, m_numFrames;
    };
    
}

