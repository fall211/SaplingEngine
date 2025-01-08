//
//  Texture.cpp
//  SaplingEngine, Sprout Renderer
//


#include "Texture.hpp"
#include "Sprout.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


namespace Sprout {
    
    Texture::~Texture() {
    }
        
    bool Texture::loadFromFile(const std::string& path, const glm::i32 numFrames) {
        int width, height, channels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data) {
            // Debug::log(("Failed to load image: %s", path.c_str()));
            return false;
        }
        
        
        m_pixels = data;
        m_width = width;
        m_height = height;
        
        m_numFrames = numFrames;
        m_frameHeight = m_height;
        m_frameWidth = m_width / m_numFrames;
        

        
        registerTexture();
        
        return true;
    }
    
    void Texture::registerTexture() {
        Window::instance->addTexture(std::shared_ptr<Texture>(this));
    }
    
    glm::vec2 Texture::getSize() {
        return glm::vec2(m_width, m_height);
    }
    
    glm::i32 Texture::getWidth() {
        return m_width;
    }
    
    glm::i32 Texture::getHeight() {
        return m_height;
    }
    
    glm::vec4 Texture::getAtlasUVs() {
        return m_atlas_uvs;
    }
    
    void Texture::setAtlasUVs(glm::vec4 uvs) {
        m_atlas_uvs = uvs;
    }
    
    glm::vec2 Texture::getFrameSize() {
        return glm::vec2(m_frameWidth, m_frameHeight);
    }
    
    glm::i32 Texture::getNumFrames()
    {
        return m_numFrames;
    }
    
    unsigned char* Texture::getPixels() {
        return m_pixels;
    }

} // namespace Sprout