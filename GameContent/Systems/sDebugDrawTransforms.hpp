//
//
//
//

#pragma once

#include "Debug.hpp"
#include "EntityManager.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "cMathAnim.hpp"

namespace System
{
    inline void DrawTransforms(EntityList& entities)
    {
        for (auto& e : entities)
        {
            if (!e->hasComponent<Comp::Transform>()) continue;
            auto& transform = e->getComponent<Comp::Transform>();
            Debug::draw_pos(transform.position);
            
        }
    }
}