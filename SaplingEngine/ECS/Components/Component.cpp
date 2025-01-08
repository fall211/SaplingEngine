//
//  Component.cpp
//  SaplingEngine
//

#include "Component.hpp"



CTransform::CTransform(const glm::vec2& positionin, const glm::vec2& velocityin)
    : position(positionin), velocity(velocityin) {}

CLifetime::CLifetime(const float lifetimein) : lifetime(lifetimein) {}

CBBox::CBBox(const float win, const float hin) : w(win), h(hin) {}

CBCircle::CBCircle(const float rin) : radius(rin) {}

CSprite::CSprite(const std::shared_ptr<Sprout::Texture>& texin)
:   texture(texin) ,
    frameSize(texin->getWidth() / numFrames)
{}

CSprite::CSprite(const std::shared_ptr<Sprout::Texture>& texin, const float animSpeed) 
:   texture(texin), 
    type(Type::Animated), 
    numFrames(texin->getNumFrames()), 
    frameSize(texin->getWidth() / numFrames),
    animationSpeed((size_t)(60.0f / animSpeed)) 
{}

// void CAnimatedSprite::setAnimationFrame(const float simTime, const float deltaTime) {

//     //Debug::log(std::to_string(fmod(averageTimeElapsed / animationSpeed, static_cast<float>(numFrames))));

//     const int index = static_cast<int>(fmod(simTime / animationSpeed, static_cast<float>(numFrames)));
//     const int size = static_cast<int>(frameSize);
//     sprite.setTextureRect(sf::IntRect(size * index, 0, size, size));
// }

CPlayerControls::CPlayerControls(const float speedIn, const int jumpStrIn) : moveSpeed(speedIn), jumpStr(jumpStrIn){}
