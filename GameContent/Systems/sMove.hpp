//
//
//
//

#pragma once

#include "EntityManager.hpp"
#include "Entity.hpp"
#include "Component.hpp"

namespace System
{
    inline void Move(EntityList& entities, float dt)
    {
        for (auto& e : entities)
        {
            auto& transform = e->getComponent<Comp::Transform>();
            
            transform.position += transform.velocity * dt;
        }
    }
}