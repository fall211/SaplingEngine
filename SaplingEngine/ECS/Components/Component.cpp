//
//  Component.cpp
//  SaplingEngine
//

#include "Component.hpp"

namespace Comp
{

    
    Transform::Transform(const glm::vec2& positionin, const glm::vec2& velocityin)
        : position(positionin), velocity(velocityin) {}
    
    Lifetime::Lifetime(const float lifetimein) : lifetime(lifetimein) {}
    
    BBox::BBox(const float win, const float hin) : w(win), h(hin) {}
        
    BCircle::BCircle(const float radiusIn) : radius(radiusIn) {}
    
    Sprite::Sprite(const std::shared_ptr<Sprout::Texture>& texin)
    :   texture(texin) ,
        frameSize(texin->getWidth() / numFrames)
    {}
    
    Sprite::Sprite(const std::shared_ptr<Sprout::Texture>& texin, const float animSpeed) 
    :   texture(texin), 
        type(Type::Animated), 
        numFrames(texin->getNumFrames()), 
        frameSize(texin->getWidth() / numFrames),
        animationSpeed((size_t)(60.0f / animSpeed)) 
    {}
    
    
    PlayerControls::PlayerControls(const float speedIn, const int jumpStrIn) : moveSpeed(speedIn), jumpStr(jumpStrIn){}

}