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
            
            if (e->hasComponent<Comp::TransformHierarchy>())
            {
                auto& hierarchy = e->getComponent<Comp::TransformHierarchy>();
                if (!hierarchy.children.empty())
                {
                    for (auto& child : hierarchy.children)
                    {
                        auto& childTransform = child->getComponent<Comp::Transform>();
                        childTransform.position = transform.position; //todo: + childTransform.localPosition;
                    }
                }
            }
            
            transform.position += transform.velocity * dt;
            

            
            if (transform.position.y > 500 && !e->hasTag("player") && !e->hasTag("pickedup"))
            {
                e->destroy();
            }
        }
    }
}