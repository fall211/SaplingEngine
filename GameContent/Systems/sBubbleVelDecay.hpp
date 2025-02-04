//
//
//
//

#pragma once

#include "Debug.hpp"
#include "EntityManager.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "cBubble.hpp"

namespace System
{
    inline void BubbleVelDecay(EntityList& entities)
    {
        for (auto& e : entities)
        {
            if (!e->hasComponent<Comp::Bubble>()) continue;
            auto& bubble = e->getComponent<Comp::Bubble>();
            auto& transform = e->getComponent<Comp::Transform>();
            
            transform.velocity *= bubble.velDecay;
            
        }
    }
}