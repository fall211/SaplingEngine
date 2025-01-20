//
//  Component.cpp
//  SaplingEngine
//

#include "Component.hpp"

#include <utility>

namespace Comp
{
    
    Transform::Transform(Inst inst, const glm::vec2& positionin, const glm::vec2& velocityin)
        :   Component(std::move(inst)),
            position(positionin), 
            velocity(velocityin) 
        {}
    
    BBox::BBox(Inst inst, const float win, const float hin) 
    :   Component(std::move(inst)),
        w(win), 
        h(hin) 
    {}
        
    BCircle::BCircle(Inst inst, const float radiusIn) 
    :   Component(std::move(inst)),
        radius(radiusIn) 
    {}
    
    Sprite::Sprite(Inst inst, const std::shared_ptr<Sprout::Texture>& texin)
        :   Component(std::move(inst)),
            texture(texin) ,
            frameSize(texin->getWidth() / numFrames)
        {}
    
    Sprite::Sprite(Inst inst, const std::shared_ptr<Sprout::Texture>& texin, const float animSpeed) 
        :   Component(std::move(inst)),
            texture(texin), 
            type(Type::Animated), 
            numFrames(texin->getNumFrames()), 
            frameSize(texin->getWidth() / numFrames),
            animationSpeed((size_t)(60.0f / animSpeed)) 
        {}
    
    SimplePlayerControls::SimplePlayerControls(Inst inst, const float speedIn, const int jumpStrIn)
        :   Component(std::move(inst)),
            moveSpeed(speedIn),
            jumpStr(jumpStrIn)
        {}

}