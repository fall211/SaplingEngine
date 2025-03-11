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
    
    inline void DrawBoundingBoxes(EntityList& entities)
    {
        for (auto& e : entities)
        {
            if (!e->hasComponent<Comp::Transform>()) continue;
            if (!e->hasComponent<Comp::BBox>()) continue;
            auto& transform = e->getComponent<Comp::Transform>();
            auto& bbox = e->getComponent<Comp::BBox>();
            if (!bbox.enabled) continue;
            auto width = bbox.w;
            auto height = bbox.h;
            auto x = transform.position.x;
            auto y = transform.position.y;
            Debug::draw_rectangle(x, y, width, height);
        }
    }
}