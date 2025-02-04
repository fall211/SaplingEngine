//
//
//
//

#pragma once

#include <functional>
#include <utility>

#include "Component.hpp"
#include "Entity.hpp"
#include "Debug.hpp"

namespace Comp
{
    class Health : public Component 
    {
        public:
            float health;
            explicit Health(Inst inst, float hp) 
            :   Component(std::move(inst)), 
                health(hp) 
            {}
            
            void OnAddToEntity() override
            {
                inst->ListenForEvent<Inst, float>("Damage", E_Damage);
            }
            
            void OnRemoveFromEntity() override
            {
                inst->RemoveEventCallback("Damage", E_Damage);
            }
            
            static void Damage(const Inst& inst, float damage)
            {
                auto& health = inst->getComponent<Health>();
                health.health -= damage;
                if (health.health <= 0)
                {
                    inst->destroy();
                }
            }
            
            private:
                std::function<void(Inst, float)> E_Damage = Damage;
    };
}