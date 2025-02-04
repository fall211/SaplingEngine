//
//
//
//

#pragma once

#include "Debug.hpp"
#include "EntityManager.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "Input/Input.hpp"
#include "cWeapon.hpp"
#include <memory>

namespace System
{
    inline void WeaponUse(EntityList& entities, std::shared_ptr<EntityManager> em, float dt)
    {
        bool fireInput = Input::isAction("fire") || Input::getMouse(Input::MouseButton::LEFT);
        
        for (auto& e : entities)
        {
            if (!e->hasComponent<Comp::Weapon>()) continue;
            auto& weapon = e->getComponent<Comp::Weapon>();
            if (!weapon.active) continue;
            weapon._time += dt;
            
            if (weapon.CanFire() && fireInput)
            {
                e->PushEvent("Fire", e, em);
            }
            
        }
    }
}