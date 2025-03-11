//
//
//
//

#pragma once

#include "Component.hpp"
#include "Entity.hpp"
#include "Debug.hpp"
#include "EntityManager.hpp"
#include "Input/Input.hpp"
#include <memory>
#include "bubbleprojectile.hpp"

namespace Comp
{
    
    class Weapon : public Component 
    {
        public:
            bool active = false;
            float fireRate = 0.1f;
            float _time = 0;
            float timeLastFired;
        
            explicit Weapon(Inst inst) 
            :   Component(std::move(inst))
            {}
            
            void OnAddToEntity() override
            {
                inst->requestAddTag("weapon");
                inst->ListenForEvent<Inst, std::shared_ptr<EntityManager>>("Fire", E_Fire);
                inst->ListenForEvent<Inst>("Pickup", E_Pickup);
                inst->ListenForEvent<Inst>("Drop", E_Drop);
            }
            
            
            void OnRemoveFromEntity() override
            {
                inst->requestRemoveTag("weapon");
                inst->RemoveEventCallback("Fire", E_Fire);
                inst->RemoveEventCallback("Pickup", E_Pickup);
                inst->RemoveEventCallback("Drop", E_Drop);
            }
            
            bool CanFire() const
            {
                return _time > timeLastFired + fireRate;
            }
            
            static void Fire(const Inst& inst, std::shared_ptr<EntityManager> em)
            {
                auto& weapon = inst->getComponent<Comp::Weapon>();

                glm::vec2 direction = glm::normalize(inst->getComponent<Comp::Transform>().position - Input::getMouseWorldPosition());                

                weapon.timeLastFired = weapon._time;
                em->instantiatePrefab<Prefab::BubbleProjectile>(inst->getComponent<Comp::Transform>().position, direction);
            }
            
            static void OnPickup(const Inst& inst)
            {
                if (inst->hasComponent<Comp::Weapon>())
                {
                    inst->getComponent<Comp::Weapon>().active = true;
                }
            }
            
            static void OnDrop(const Inst& inst)
            {
                if (inst->hasComponent<Comp::Weapon>())
                {
                    inst->getComponent<Comp::Weapon>().active = false;
                }
            }

            
        private:
            std::function<void(Inst, std::shared_ptr<EntityManager>)> E_Fire = Weapon::Fire;
            std::function<void(Inst)> E_Pickup = Weapon::OnPickup;
            std::function<void(Inst)> E_Drop = Weapon::OnDrop;
    };
    
    
}