//
//  Component.cpp
//  SaplingEngine
//

#include "Component.hpp"

namespace Comp
{
    
    Transform::Transform(Inst inst, const glm::vec2& positionin, const glm::vec2& velocityin)
        :   Component(inst),
            position(positionin), 
            velocity(velocityin) 
        {}
    
    BBox::BBox(Inst inst, const float win, const float hin) 
    :   Component(inst),
        w(win), 
        h(hin) 
    {}
        
    BCircle::BCircle(Inst inst, const float radiusIn) 
    :   Component(inst),
        radius(radiusIn) 
    {}
    
    Sprite::Sprite(Inst inst, const std::shared_ptr<Sprout::Texture>& texin)
        :   Component(inst),
            texture(texin) ,
            frameSize(texin->getWidth() / numFrames)
        {}
    
    Sprite::Sprite(Inst inst, const std::shared_ptr<Sprout::Texture>& texin, const float animSpeed) 
        :   Component(inst),
            texture(texin), 
            type(Type::Animated), 
            numFrames(texin->getNumFrames()), 
            frameSize(texin->getWidth() / numFrames),
            animationSpeed((size_t)(60.0f / animSpeed)) 
        {}
    
    PlayerControls::PlayerControls(Inst inst, const float speedIn, const int jumpStrIn)
        :   Component(inst),
            moveSpeed(speedIn),
            jumpStr(jumpStrIn)
        {}

}