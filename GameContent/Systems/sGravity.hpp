//
//
//
//


#pragma once

#include "Component.hpp"
#include "Debug.hpp"
#include "EntityManager.hpp"
#include "Entity.hpp"
#include "cGravity.hpp"

namespace System
{
    inline void Gravity(const EntityList& entities)
    {
        for (const auto& entity : entities)
        {
            if (entity->hasComponent<Comp::Gravity>())
            {
                auto& transform = entity->getComponent<Comp::Transform>();
                transform.velocity.y += entity->getComponent<Comp::Gravity>().gravity;
            }
        }
    }
}