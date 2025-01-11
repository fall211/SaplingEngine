//
//
//
//

#pragma once

#include "EntityManager.hpp"
#include "Entity.hpp"
#include "cRotate.hpp"

namespace Systems
{
    
    inline void Rotate(EntityList& entities, double dt)
    {
        for (const auto& e : entities)
        {
            if (e->hasComponent<Comp::Transform>() && e->hasComponent<Comp::Rotate>())
            {
                auto& transform = e->getComponent<Comp::Transform>();
                transform.rotation += e->getComponent<Comp::Rotate>().rotationSpeed * dt;
            }
        }
    }
    
}