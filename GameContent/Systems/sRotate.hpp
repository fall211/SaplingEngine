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
    
    inline void sRotate(EntityList& entities, double dt)
    {
        for (const auto& e : entities)
        {
            if (e->hasComponent<CTransform>() && e->hasComponent<CRotate>())
            {
                auto& transform = e->getComponent<CTransform>();
                transform.rotation += e->getComponent<CRotate>().rotationSpeed * dt;
            }
        }
    }
    
}