//
//
//
//

#pragma once

#include <functional>
#include <utility>

#include "Audio/AudioEngine.hpp"
#include "Component.hpp"
#include "Entity.hpp"
#include "Debug.hpp"
#include "cDamage.hpp"

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
                inst->ListenForEvent<Inst, int>("TakeDamage", E_TakeDamage);
                inst->ListenForEvent<Inst, Inst>("TriggerEnter", E_OnTriggerEnter);
            }
            
            void OnRemoveFromEntity() override
            {
                inst->RemoveEventCallback("TakeDamage", E_TakeDamage);
                inst->RemoveEventCallback("TriggerEnter", E_OnTriggerEnter);
            }
            
            static void TakeDamage(const Inst& inst, int damage)
            {
                auto& health = inst->getComponent<Health>();
                health.health -= damage;
                
                std::string message = "Entity " + std::to_string(inst->getId()) + " took " + std::to_string(damage) + " damage. Current health: " + std::to_string(health.health);
                Debug::log(message);
                
                AudioEngine::getInstance()->playSound("hurtPlayer");
                
                if (inst->hasComponent<Sprite>())
                {
                    auto& sprite = inst->getComponent<Sprite>();
                    sprite.setColorOverride(glm::vec4(1, 0, 0, 0.25), 3);
                }
                
                if (health.health <= 0)
                {
                    inst->PushEvent("Death", inst);
                }
            }
            
            static void OnTriggerEnter(const Inst& inst, const Inst& other)
            {
                if (inst->hasComponent<Comp::Health>() && other->hasComponentEnabled<Damage>())
                {
                    if (inst->hasTag("player") && other->hasTag("enemy"))
                    {
                        int damage = other->getComponent<Damage>().damage;
                        inst->PushEvent("TakeDamage", inst, damage);
                    }
                }
            }
            
            private:
                std::function<void(Inst, int)> E_TakeDamage = TakeDamage;
                std::function<void(Inst, Inst)> E_OnTriggerEnter = OnTriggerEnter;
    };
}