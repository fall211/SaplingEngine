//
//
//
//

#pragma once

#include "Debug.hpp"
#include "EntityManager.hpp"
#include "Entity.hpp"
#include "Component.hpp"

namespace System
{
    inline void Move(EntityList& entities, float dt)
    {
        for (auto& e : entities)
        {
            if (!e->hasComponent<Comp::Transform>()) continue;
            auto& transform = e->getComponent<Comp::Transform>();
            
            transform.position += transform.velocity * dt;
            
            if (e->hasComponent<Comp::TransformHierarchy>())
            {
                auto& hierarchy = e->getComponent<Comp::TransformHierarchy>();
                if (hierarchy.parent != nullptr)
                {
                    auto& parentTransform = hierarchy.parent->getComponent<Comp::Transform>();
                    transform.position = parentTransform.position;
                }
            }
        }
    }
}