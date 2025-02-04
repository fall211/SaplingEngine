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
    inline void MathAnim(EntityList& entities, float time)
    {
        for (auto& e : entities)
        {
            if (!e->hasComponent<Comp::MathAnim>() || !e->hasComponent<Comp::Sprite>()) continue;
            auto& mathAnim = e->getComponent<Comp::MathAnim>();
            auto& sprite = e->getComponent<Comp::Sprite>();
            
            float y = mathAnim.easeInOutSine(time) - 0.5f;
            sprite.transformOffset.y = y * 10;
        }
    }
}