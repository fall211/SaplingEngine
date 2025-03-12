//
//
//
//

#pragma once

#include "Debug.hpp"
#include "EntityManager.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "cHealth.hpp"
#include "cLifetime.hpp"

namespace System
{
    inline void Lifetime(EntityList& entities, float dt)
    {
        for (auto& e : entities)
        {
            if (e->hasComponent<Comp::Lifetime>())
            {
                auto& lifetime = e->getComponent<Comp::Lifetime>();
                lifetime.lifetime -= dt;
                
                if (lifetime.lifetime <= 0)
                {
                    e->destroy();
                }
            }

        }
    }
}