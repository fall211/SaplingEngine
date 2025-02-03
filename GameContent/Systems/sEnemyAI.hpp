//
//
//
//

#pragma once

#include "EntityManager.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "cEnemyMovement.hpp"

namespace System
{
    inline void EnemyAI(EntityList& entities, Inst& player)
    {
        auto& playerTransform = player->getComponent<Comp::Transform>();
        
        for (auto& e : entities)
        {
            if (!e->hasComponent<Comp::Transform>() || !e->hasComponent<Comp::EnemyMovement>()) continue;
            auto& transform = e->getComponent<Comp::Transform>();
            auto& movement = e->getComponent<Comp::EnemyMovement>();
            
            auto distance = abs(playerTransform.position.x - transform.position.x);
            if (distance < movement.detectRange)
            {
                if (distance < 10)
                {
                    transform.velocity.x = 0;
                }
                else 
                {
                    auto sign = playerTransform.position.x < transform.position.x ? -1 : 1;
                    transform.velocity.x = movement.speed * sign;
                }
            }
            else 
            {
                transform.velocity.x = 0;
            }
            
            
        }
    }
}