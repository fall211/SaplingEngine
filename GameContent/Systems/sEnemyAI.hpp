//
//
//
//

#pragma once

#include "Debug.hpp"
#include "EntityManager.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "cEnemyMovement.hpp"
#include "cMapData.hpp"

namespace System
{
    inline void EnemyAI(EntityList& entities, Inst& player, Inst& map)
    {
        auto& playerTransform = player->getComponent<Comp::Transform>();
        auto& mapData = map->getComponent<Comp::MapData>();

        for (auto& e : entities)
        {
            if (!e->hasComponent<Comp::Transform>() || !e->hasComponent<Comp::EnemyMovement>()) continue;
            auto& transform = e->getComponent<Comp::Transform>();
            auto& movement = e->getComponent<Comp::EnemyMovement>();
            
            // Check for edge before moving
            glm::vec2 groundCheck = transform.position;
            groundCheck.x += movement.moveDirection * 12;
            groundCheck.y += 12; 
            
            int tile = mapData.getTile(groundCheck.x, groundCheck.y);
            bool hasGround = tile != -1 && tile != 13;
            
            auto distance = abs(playerTransform.position.x - transform.position.x);
            if (distance < movement.detectRange)
            {
                if (distance < 10)
                {
                    transform.velocity.x = 0;
                }
                else 
                {
                    movement.moveDirection = playerTransform.position.x < transform.position.x ? -1 : 1;

                    
                    if (hasGround) {
                        transform.velocity.x = movement.speed * movement.moveDirection;
                    }
                    else {
                        transform.velocity.x = 0;
                    }
                }
            }
            else 
            {
                if (hasGround) {
                    transform.velocity.x = movement.speed / 2 * movement.moveDirection;
                }
                else {
                    movement.moveDirection *= -1;
                }
            }
        }
    }
}
