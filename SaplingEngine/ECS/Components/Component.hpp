//
//  Component.hpp
//  ECS_Engine
//
//  Created by Tuukka Virtanen on 1/6/24.
//

#pragma once


#include "SaplingEngine.hpp"
#include "Physics/Math.hpp"

class Component{
public:
    bool has = false;
    Component()= default;
    virtual ~Component()= default;
};

class CTransform final : public Component{
    public:
        Vector2 position = Vector2(0, 0);
        Vector2 velocity = Vector2(0, 0);

        CTransform(const Vector2& positionin, const Vector2& velocityin);
};

class CShape final : public Component{
    public:
        sf::RectangleShape rectShape;
        sf::CircleShape circleShape;
        explicit CShape(sf::RectangleShape shapeIn);
        explicit CShape(sf::CircleShape shapeIn);
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
    sf::Sprite sprite;
    explicit CSprite(const std::shared_ptr<sf::Texture>& texin);
};

class CAnimatedSprite final : public Component {
public:
    sf::Sprite sprite;
    size_t numFrames;
    size_t frameSize;
    float animationSpeed;

    explicit CAnimatedSprite(const std::shared_ptr<sf::Texture>& texture, float animSpeed = 0.5f);
    void setAnimationFrame(float simTime, float deltaTime);
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

