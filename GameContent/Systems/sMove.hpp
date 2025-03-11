//
//
//
//

#pragma once

#include "Debug.hpp"
#include "EntityManager.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "Physics.hpp"
#include "SpatialGrid.hpp"

namespace System
{
    inline void Move(std::shared_ptr<EntityManager>& entityManager, EntityList entitiesInRange, float dt)
    {
        int numCollisionChecks = 0;
        
        auto dynamicEntities = entityManager->getEntitiesInRange("dynamic", entitiesInRange);
        
        SpatialGrid& grid = entityManager->getSpatialGrid();

        
        
        for (auto& e : dynamicEntities)
        {
            if (!e->hasComponent<Comp::Transform>()) continue;
            auto& transform = e->getComponent<Comp::Transform>();
            
            grid.updateEntity(e);
            
            if (e->hasComponent<Comp::TransformHierarchy>())
            {
                auto& hierarchy = e->getComponent<Comp::TransformHierarchy>();
                if (!hierarchy.children.empty())
                {
                    for (auto& child : hierarchy.children)
                    {
                        auto& childTransform = child->getComponent<Comp::Transform>();
                        childTransform.position = transform.position; //todo: + childTransform.localPosition;
                        grid.updateEntity(child);
                    }
                }
            }
            
            if (!e->hasComponentEnabled<Comp::BBox>())
            {
                transform.position += transform.velocity * dt;
            }
            else 
            {                
                auto potentialCollisions = grid.getPotentialCollisions(e);
                // horizontal movement
                transform.position.x += transform.velocity.x * dt;
                grid.updateEntity(e);

                // horizontal collision
                // get colliding entities and resolve
                Physics2D::CollisionData collision;
                for (auto& physicsEntity : potentialCollisions)
                {
                    if (e->getId() == physicsEntity->getId() ||
                        !physicsEntity->hasComponentEnabled<Comp::BBox>()
                    ) continue;
                    
                    auto& bbox1 = e->getComponent<Comp::BBox>();
                    auto& bbox2 = physicsEntity->getComponent<Comp::BBox>();
                    collision = Physics2D::collisionData(e, physicsEntity);
                    
                    numCollisionChecks++;
                    
                    if (collision.type == Physics2D::CollisionType::NONE)
                    {
                        if (!bbox1.collidingWith.empty())
                        {
                            if (bbox1.collidingWith.find(&bbox2) != bbox1.collidingWith.end())
                            {
                                bbox1.collidingWith.erase(&bbox2);
                                if (bbox1.interactWithTriggers)     e->PushEvent("TriggerExit", e, physicsEntity);
                                if (bbox1.collisionEventsEnabled)   e->PushEvent("CollisionExit", e, physicsEntity);
                            }
                        }
                        
                        continue;
                    };
                    
                    if (collision.triggerEvent || bbox1.collisionEventsEnabled)
                    {
                        if (bbox1.collidingWith.find(&bbox2) == bbox1.collidingWith.end())
                        {
                            // not in the set, so we just entered the trigger
                            bbox1.collidingWith.insert(&bbox2);
                            if (collision.triggerEvent)         e->PushEvent("TriggerEnter", e, physicsEntity);
                            if (bbox1.collisionEventsEnabled)   e->PushEvent("CollisionEnter", e, physicsEntity);
                        }
                        else 
                        {
                            // we are still colliding with the trigger
                            if (collision.triggerEvent)         e->PushEvent("TriggerStay", e, physicsEntity);
                            if (bbox1.collisionEventsEnabled)   e->PushEvent("CollisionStay", e, physicsEntity);
                        }
                        
                    }
                    
                    if (collision.type == Physics2D::CollisionType::DYNAMIC_STATIC)
                    {
                        if (transform.velocity.x > 0)
                        {
                            transform.position.x -= collision.overlap.x;
                        }
                        else if (transform.velocity.x < 0)
                        {
                            transform.position.x += collision.overlap.x;
                        }
                    }
                    else if (collision.type == Physics2D::CollisionType::DYNAMIC_DYNAMIC && !collision.trigger)
                    {
                        auto& transform2 = physicsEntity->getComponent<Comp::Transform>();
                        if (transform.velocity.x > 0)
                        {
                            transform.position.x -= collision.overlap.x / 2.0f;
                            transform2.position.x += collision.overlap.x / 2.0f;
                        }
                        else if (transform.velocity.x < 0)
                        {
                            transform.position.x += collision.overlap.x / 2.0f;
                            transform2.position.x -= collision.overlap.x / 2.0f;
                        }
                    }
                }
                
                // vertical movement
                transform.position.y += transform.velocity.y * dt;
                grid.updateEntity(e);

                // vertical collision
                // get colliding entities and resolve
                for (auto& physicsEntity : potentialCollisions)
                {
                    if (e->getId() == physicsEntity->getId() ||
                        !physicsEntity->hasComponentEnabled<Comp::BBox>()
                    ) continue;
                    
                    collision = Physics2D::collisionData(e, physicsEntity);
                    
                    numCollisionChecks++;
                    
                    if (collision.type == Physics2D::CollisionType::DYNAMIC_STATIC)
                    {
                        if (transform.velocity.y > 0)
                        {
                            transform.position.y -= collision.overlap.y;
                            transform.velocity.y = 0;
                            e->PushEvent("FloorCollision", e);

                        }
                        else if (transform.velocity.y < 0)
                        {
                            transform.position.y += collision.overlap.y;
                            if (e->hasTag("player"))
                            {
                                e->PushEvent("CeilingCollision", e);
                            }
                        }
                    }
                    else if (collision.type == Physics2D::CollisionType::DYNAMIC_DYNAMIC && !collision.trigger)
                    {
                        
                    }
                }
                
            }


            if (transform.reset)
            {
                Debug::log("Resetting entity");
                transform.position = glm::vec2(0, 0);
                transform.velocity = glm::vec2(0, 0);
                transform.rotation = 0;
                transform.reset = false;
                grid.updateEntity(e);
            }
        }
        
        // Debug::log("Number of collision checks: " + std::to_string(numCollisionChecks));
        
    }
}