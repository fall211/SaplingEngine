//
//  Component.hpp
//  SaplingEngine
//

#pragma once


#include "Renderer/Texture.hpp"
#include "glm/glm.hpp"
#include <memory>

class Component{
public:
    bool has = false;
    Component()= default;
    virtual ~Component()= default;
};

class CTransform final : public Component{
    public:
        glm::vec2 position = glm::vec2(0, 0);
        glm::vec2 velocity = glm::vec2(0, 0);
        glm::vec4 rotation = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

        CTransform(const glm::vec2& positionin, const glm::vec2& velocityin);
};


class CLifetime final : public Component {
public:
    float lifetime = 1.0f;
    explicit CLifetime(float lifetimein);
};

class CBBox final : public Component {
public:
    float w = 1.0f;
    float h = 1.0f;
    CBBox(float win, float hin);
};

class CBCircle final : public Component {
public:
    float radius = 1;
    explicit CBCircle(float rin);
};

class CSprite final : public Component {
public:
    std::shared_ptr<Sprout::Texture> texture;
    explicit CSprite(const std::shared_ptr<Sprout::Texture>& texin);
    explicit CSprite(const std::shared_ptr<Sprout::Texture>& texin, float animSpeed);
    
    enum class Layer {
        Background,
        Midground,
        Foreground,
        UserInterface,
        Count
    };
    
    void setLayer(Layer layerIn) { layer = layerIn; }
    
    enum class Type {
        Static,
        Animated
    };
    
    Type type = Type::Static;
    Layer layer = Layer::Midground;
    
    // animated frame stuff
    size_t numFrames = 1;
    size_t currentFrame = 0;
    size_t frameSize;
    size_t animationSpeed = 60;
};

class CFollowMouse final : public Component {
};

class CPlayerControls final : public Component {
public:
    float moveSpeed = 0.0f;
    int jumpStr;
    bool grounded = true;
    CPlayerControls(float speedIn, int jumpStrIn);
};

