//
//
//
//

#pragma once

#include "Debug.hpp"
#include "EntityManager.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "cBrain.hpp"
#include "cMapData.hpp"

namespace System
{
    inline void EnemyAI(std::shared_ptr<EntityManager>& entManager, float dt)
    {
        auto& entities = entManager->getEntities("enemy");
        auto& player = entManager->getEntities("player").front();
        auto& map = entManager->getEntities("map").front();
        
        auto& playerTransform = player->getComponent<Comp::Transform>();
        auto& mapData = map->getComponent<Comp::MapData>();

        for (auto& e : entities)
        {
            if (!e->hasComponent<Comp::Transform>() || !e->hasComponent<Comp::Brain>()) continue;
            auto& transform = e->getComponent<Comp::Transform>();
            auto& brain = e->getComponent<Comp::Brain>();
            brain.Update(dt);
            
            // Check for edge before moving
            glm::vec2 groundCheck = transform.position;
            groundCheck.x += brain.moveDirection * 12;
            groundCheck.y += 12; 
            
            glm::vec2 wallCheck = transform.position;
            wallCheck.x += brain.moveDirection * 15;

            
            int tile = mapData.getTile(groundCheck.x, groundCheck.y);
            bool hasGround = tile != -1;

            
            tile = mapData.getTile(wallCheck.x, wallCheck.y);
            bool hasWall = tile != -1;
            
            auto distance = abs(playerTransform.position.x - transform.position.x);
            
            switch (brain.state)
            {
                case Comp::Brain::State::IDLE:
                    if (hasGround)
                    {
                        brain.state = Comp::Brain::State::PATROL;
                    }
                    break;
                    
                case Comp::Brain::State::PATROL:
                    if (hasGround) {
                        transform.velocity.x = brain.speed / 2 * brain.moveDirection;
                    }
                    else {
                        brain.moveDirection *= -1;
                    }
                    if (distance < brain.detectRange) {
                        brain.state = Comp::Brain::State::CHASE;
                    }
                    break;
                    
                case Comp::Brain::State::CHASE:
                    brain.moveDirection = playerTransform.position.x < transform.position.x ? -1 : 1;

                    if (hasGround && hasWall && brain.CanJump()) {
                        transform.velocity.x = brain.speed * brain.moveDirection;
                        e->PushEvent("Jump", e);
                    }
                    else if (hasGround && !hasWall) {
                        transform.velocity.x = brain.speed * brain.moveDirection;
                    }
                    else if (!hasGround)
                    {
                        if (brain.CanJump()) 
                        {
                            transform.velocity.x = brain.speed * brain.moveDirection;
                            e->PushEvent("Jump", e);
                        }
                        else if (brain.grounded)
                        {
                            transform.velocity.x = 0;
                        }
                    }
                    
                    if (distance < brain.attackRange && brain.grounded) {
                        brain.state = Comp::Brain::State::PRE_ATTACK;
                    }
                    else if (distance > brain.stopPursuitRange)
                    {
                        brain.state = Comp::Brain::State::IDLE;
                    }

                    break;
                    
                case Comp::Brain::State::PRE_ATTACK:
                    transform.velocity.x = 0;
                    
                    if (distance > brain.attackRange * 2)
                    {
                        brain.state = Comp::Brain::State::IDLE;
                    }
                    
                    if (brain.CanAttack())
                    {
                        brain.state = Comp::Brain::State::ATTACK;
                    }

                
                    break;
                    
                case Comp::Brain::State::ATTACK:
                    if (brain.CanAttack())
                    {
                        brain.moveDirection = playerTransform.position.x < transform.position.x ? -1 : 1;
                        e->PushEvent("Attack", e);
                    }
                    else
                    {
                        auto moveTowards = [](float current, float target, float maxDelta) -> float {
                            if (std::abs(target - current) <= maxDelta) {
                                return target;
                            }
                            return current + glm::sign(target - current) * maxDelta;
                        };
                        transform.velocity.x = moveTowards(transform.velocity.x, 0, 600 * dt);
                    }
                    if (abs(transform.velocity.x) < 10)
                        {
                            e->PushEvent("FinishAttack", e);
                            brain.state = Comp::Brain::State::IDLE;
                        }

                    
                    break;
                    
                case Comp::Brain::State::FLEE:

                    break;
                    
                case Comp::Brain::State::DIE:

                    break;
            }
        }
    }
}
