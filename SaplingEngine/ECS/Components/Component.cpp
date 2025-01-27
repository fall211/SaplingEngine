//
//  Component.cpp
//  SaplingEngine
//

#include "Component.hpp"
#include "Entity.hpp"

#include <utility>

namespace Comp
{
    
    Transform::Transform(Inst inst, const glm::vec2& positionin, const glm::vec2& velocityin)
        :   Component(std::move(inst)),
            position(positionin), 
            velocity(velocityin) 
        {}
    
    Transform::Transform(Inst inst, const glm::vec2& positionin)
        :   Component(std::move(inst)),
            position(positionin) 
        {}
    
    BBox::BBox(Inst inst, const float win, const float hin) 
    :   Component(std::move(inst)),
        h(hin), 
        w(win) 
    {}
    
    void BBox::OnAddToEntity()
    {
        inst->requestAddTag("hascollider");
    }
    void BBox::OnRemoveFromEntity()
    {
        inst->requestRemoveTag("hascollider");
    }
        
    BCircle::BCircle(Inst inst, const float radiusIn) 
    :   Component(std::move(inst)),
        radius(radiusIn) 
    {}
    
    void BCircle::OnAddToEntity()
    {
        inst->requestAddTag("hascollider");
    }
    
    void BCircle::OnRemoveFromEntity()
    {
        inst->requestRemoveTag("hascollider");
    }
    
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
        
    void Sprite::OnAddToEntity()
    {
        inst->requestAddTag("drawable");
    }
    
    void Sprite::OnRemoveFromEntity()
    {
        inst->requestRemoveTag("drawable");
    }
    
    SimplePlayerControls::SimplePlayerControls(Inst inst, const float speedIn, const int jumpStrIn)
        :   Component(std::move(inst)),
            moveSpeed(speedIn),
            jumpStr(jumpStrIn)
        {}
        
    TransformHierarchy::TransformHierarchy(Inst inst)
        :   Component(std::move(inst))
        {}
        
    void TransformHierarchy::setParent(Inst newParent)
    {
        if (parent != nullptr)
        {
            // Remove this instance from the current parent's children
            auto& siblings = parent->getComponent<TransformHierarchy>().children;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), inst), siblings.end());
        }
    
        parent = std::move(newParent);
    
        if (parent != nullptr)
        {
            // Add this instance to the new parent's children
            parent->getComponent<TransformHierarchy>().children.push_back(inst);
        }    }
    
    void TransformHierarchy::removeParent()
    {
        if (parent != nullptr)
        {
            // Remove this instance from the current parent's children
            auto& siblings = parent->getComponent<TransformHierarchy>().children;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), inst), siblings.end());
            parent = nullptr;
        }    }
    
    void TransformHierarchy::addChild(const Inst& child)
    {
        auto& childHierarchy = child->getComponent<TransformHierarchy>();
        if (childHierarchy.parent != nullptr)
        {
            // Remove the child from its current parent
            childHierarchy.removeParent();
        }
    
        children.push_back(child);
        childHierarchy.setParent(inst);    }
    
    void TransformHierarchy::removeChild(const Inst& child)
    {
        auto& childHierarchy = child->getComponent<TransformHierarchy>();
        if (childHierarchy.parent == inst)
        {
            children.erase(std::remove(children.begin(), children.end(), child), children.end());
            childHierarchy.removeParent();
        }
    }
}