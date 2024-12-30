//
//  Component.hpp
//  SaplingEngine
//

#pragma once


#include "SaplingEngine.hpp"
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
};

// class CAnimatedSprite final : public Component {
// public:
//     sf::Sprite sprite;
//     size_t numFrames;
//     size_t frameSize;
//     float animationSpeed;

//     explicit CAnimatedSprite(const std::shared_ptr<sf::Texture>& texture, float animSpeed = 0.5f);
//     void setAnimationFrame(float simTime, float deltaTime);
// };

class CFollowMouse final : public Component {
};

class CPlayerControls final : public Component {
public:
    float moveSpeed = 0.0f;
    int jumpStr;
    bool grounded = true;
    CPlayerControls(float speedIn, int jumpStrIn);
};

