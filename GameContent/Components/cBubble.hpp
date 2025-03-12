//
//
//
//

#pragma once

#include "Component.hpp"
#include "Entity.hpp"
#include "Debug.hpp"
#include "cHealth.hpp"

namespace Comp
{
    
    class Bubble : public Component 
    {
        public:
            float initialVel = 100;
            float velDecay = 0.999;
            float lifetime = 1.5;
            explicit Bubble(Inst inst) 
            :   Component(std::move(inst))
            {}
            
            
            void OnAddToEntity() override
            {
                inst->ListenForEvent<Inst, Inst>("CollisionEnter", E_CollisionEnter);
            }
            
            
            void OnRemoveFromEntity() override
            {
                inst->RemoveEventCallback("CollisionEnter", E_CollisionEnter);
            }
            
            static void CollisionEnter(const Inst& inst, const Inst& other)
            {
                if (inst->hasComponent<Bubble>())
                {
                    if (other->hasComponent<Health>() && other->hasTag("enemy"))
                    {
                        int damage = 1;
                        other->PushEvent("TakeDamage", other, damage);
                    }
                    
                    if (!other->hasTag("player"))   inst->destroy();
                }
            }

            
        private:
            std::function<void(Inst, Inst)> E_CollisionEnter = Bubble::CollisionEnter;
            
    };
    
    
}