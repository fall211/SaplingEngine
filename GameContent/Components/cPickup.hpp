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
                inst->ListenForEvent<Inst>("Pickup", E_Pickup);
                inst->ListenForEvent<Inst>("Drop", E_Drop);
            }
            
            
            void OnRemoveFromEntity() override
            {
                inst->requestRemoveTag("pickup");
                inst->RemoveEventCallback("Pickup", E_Pickup);
                inst->RemoveEventCallback("Drop", E_Drop);
            }
            
            static void OnPickup(const Inst& inst)
            {
                if (inst->hasComponent<Sprite>())
                {
                    inst->getComponent<Sprite>().setLayer(Sprite::Layer::Foreground);
                }
                if (inst->hasComponent<BBox>())
                {
                    inst->getComponent<BBox>().isStatic = true;
                }
                if (inst->hasComponent<Gravity>())
                {
                    inst->removeComponent<Gravity>();
                }
            }
            
            static void OnDrop(const Inst& inst)
            {
                if (inst->hasComponent<Sprite>())
                {
                    inst->getComponent<Sprite>().setLayer(Sprite::Layer::Midground);
                }
                if (inst->hasComponent<BBox>())
                {
                    inst->getComponent<BBox>().isStatic = false;
                }
                if (!inst->hasComponent<Gravity>())
                {
                    inst->addComponent<Gravity>();
                }
            }
            
        private:
            std::function<void(Inst)> E_Pickup = Pickup::OnPickup;
            std::function<void(Inst)> E_Drop = Pickup::OnDrop;
    };
    
    
}