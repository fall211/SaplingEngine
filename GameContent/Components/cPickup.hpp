//
//
//
//

#pragma once

#include "Component.hpp"
#include "Entity.hpp"
#include "Debug.hpp"
#include "cGravity.hpp"

namespace Comp
{
    
    class Pickup : public Component 
    {
        public:
            explicit Pickup(Inst inst) 
            :   Component(std::move(inst))
            {}
            
            void OnAddToEntity() override
            {
                inst->requestAddTag("pickup");
                inst->ListenForEvent<Inst, Inst>("Pickup", E_Pickup);
                inst->ListenForEvent<Inst>("Drop", E_Drop);
            }
            
            
            void OnRemoveFromEntity() override
            {
                inst->requestRemoveTag("pickup");
                inst->RemoveEventCallback("Pickup", E_Pickup);
                inst->RemoveEventCallback("Drop", E_Drop);
            }
            
            static void OnPickup(const Inst& inst, const Inst& picker)
            {
                if (inst->hasComponent<TransformHierarchy>() && picker->hasComponent<TransformHierarchy>())
                {
                    auto& transform = inst->getComponent<TransformHierarchy>();
                    transform.setParent(picker);
                    inst->requestAddTag("pickedup");
                }
                if (inst->hasComponent<Transform>())
                {
                    inst->getComponent<Transform>().pivot = Sprout::Pivot::BOTTOM_LEFT;
                    inst->getComponent<Transform>().velocity = glm::vec2(0, 0);
                }
                if (inst->hasComponent<Sprite>())
                {
                    inst->getComponent<Sprite>().setLayer(Sprite::Layer::Foreground);
                }
                if (inst->hasComponent<BBox>())
                {
                    inst->getComponent<BBox>().enabled = false;
                }
                if (inst->hasComponent<Gravity>())
                {
                    inst->removeComponent<Gravity>();
                }
            }
            
            static void OnDrop(const Inst& inst)
            {
                if (inst->hasComponent<TransformHierarchy>())
                {
                    auto& transform = inst->getComponent<TransformHierarchy>();
                    transform.removeParent();
                    inst->requestRemoveTag("pickedup");
                }
                if (inst->hasComponent<Transform>())
                {
                    inst->getComponent<Transform>().pivot = Sprout::Pivot::CENTER;
                }
                if (inst->hasComponent<Sprite>())
                {
                    inst->getComponent<Sprite>().setLayer(Sprite::Layer::Midground);
                }
                if (inst->hasComponent<BBox>())
                {
                    inst->getComponent<BBox>().enabled = true;
                }
                if (!inst->hasComponent<Gravity>())
                {
                    inst->addComponent<Gravity>();
                }
            }
            
        private:
            std::function<void(Inst, Inst)> E_Pickup = Pickup::OnPickup;
            std::function<void(Inst)> E_Drop = Pickup::OnDrop;
    };
    
    
}