//
//
//
//

#include "PrototypeScene.hpp"
#include "Component.hpp"
#include "Debug.hpp"
#include "EntityManager.hpp"
#include "cCoyoteTime.hpp"
#include "cGravity.hpp"
#include "cLowestY.hpp"
#include "cPlayerController.hpp"
#include "cRotate.hpp"
#include <string>

PrototypeScene::PrototypeScene(Engine& engine) : Scene(engine)
{
    init();
}

void PrototypeScene::init()
{
    m_input->makeAction("jump", {SAPP_KEYCODE_SPACE});
    m_input->makeAxis("horizontal", SAPP_KEYCODE_D, SAPP_KEYCODE_A);
    
    sSpawn();
}

void PrototypeScene::update()
{
    sMove(m_entityManager->getEntities());

    sResolveCollisions(m_entityManager->getEntities());
    
    
    auto entitiesWithSprite = m_entityManager->getEntitiesByComponent<Comp::Sprite>();
    sRender(entitiesWithSprite);
}

void PrototypeScene::sSpawn()
{
    auto player = m_entityManager->instantiatePrefab<Prefab::Player>();
    
    for (int i = 0; i < 20; i++)
    {
        auto obstacle = m_entityManager->addEntity({"static", "obstacle"});
        int x = 64 * i - 620;
        obstacle->addComponent<Comp::Transform>(glm::vec2(x, 300), glm::vec2(0, 0));
        obstacle->addComponent<Comp::Sprite>(AssetManager::getTexture("test"));
        obstacle->addComponent<Comp::BBox>(64, 64);    
    }
    for (int i = 0; i < 20; i++)
    {
        auto obstacle = m_entityManager->addEntity({"static", "obstacle"});
        int x = 64 * i - 800;
        obstacle->addComponent<Comp::Transform>(glm::vec2(x, 150), glm::vec2(0, 0));
        obstacle->addComponent<Comp::Sprite>(AssetManager::getTexture("test"));
        obstacle->addComponent<Comp::BBox>(64, 64);    
    }
    
    auto obstacle = m_entityManager->addEntity({"static", "obstacle"});
    obstacle->addComponent<Comp::Transform>(glm::vec2(250, 0), glm::vec2(0, 0));
    obstacle->addComponent<Comp::Sprite>(AssetManager::getTexture("test"));
    obstacle->addComponent<Comp::BBox>(64, 64).isStatic = false;
    obstacle->addComponent<Comp::Gravity>();
}

void PrototypeScene::sMove(const EntityList& entities)
{
    for (const auto& entity : entities)
    {
        auto& transform = entity->getComponent<Comp::Transform>();
        
        if (entity->hasTag("playercontrolled") && entity->hasComponent<Comp::Moves>())
        {
            auto& moves = entity->getComponent<Comp::Moves>();
            
            if (m_input->isActionDown("jump"))
            {
                entity->PushEvent("jump");
            }
            
            if (m_input->getAxis("horizontal") > 0)
            {
                transform.velocity.x = moves.moveSpeed;
            }
            else if (m_input->getAxis("horizontal") < 0)
            {
                transform.velocity.x = -moves.moveSpeed;
            }
            else
            {
                transform.velocity.x = 0;
            }
            
        }
        
        if (entity->hasComponent<Comp::Gravity>() && entity->hasTag("gravityAffected"))
        {
            transform.velocity.y += entity->getComponent<Comp::Gravity>().gravity * m_engine.deltaTime();
        }
        
        if (entity->hasComponent<Comp::LowestY>())
        {
            if (transform.position.y > entity->getComponent<Comp::LowestY>().y_level)
            {
                transform.position.y = entity->getComponent<Comp::LowestY>().y_level;
                transform.velocity.y = 0;
                
                if (entity->hasComponent<Comp::Moves>())
                {
                    entity->getComponent<Comp::Moves>().grounded = true;
                }
            }
        }
        
        transform.position += transform.velocity * m_engine.deltaTime();
    }
}

void PrototypeScene::sResolveCollisions(const EntityList& entities)
{
    for (const auto& ent1 : entities)
    {
        bool collided = false;
        if (!ent1->hasComponent<Comp::BBox>()) continue;
        for (const auto& ent2: entities)
        {
            if (ent1->getId() == ent2->getId() || !ent2->hasComponent<Comp::BBox>()) continue;
            
            glm::vec2 overlap = Physics2D::bBoxCollision(ent1, ent2);
            
            if (overlap.x != 0 || overlap.y != 0)
            {
                collided = true;
                auto& transform1 = ent1->getComponent<Comp::Transform>();
                auto& transform2 = ent2->getComponent<Comp::Transform>();
                
                auto& bbox1 = ent1->getComponent<Comp::BBox>();
                auto& bbox2 = ent2->getComponent<Comp::BBox>();
                                
                if (bbox1.isStatic) continue;
                
                if (overlap.x > overlap.y) // resolve collision on the y axis
                {
                    if (bbox2.isStatic)
                    {
                        if (transform1.position.y < transform2.position.y) transform1.position.y -= overlap.y;
                        else transform1.position.y += overlap.y;
                        
                        transform1.velocity.y = 0;
                    }
                    else
                    {
                        if (transform1.position.y < transform2.position.y) 
                        {
                            transform1.position.y -= overlap.y / 2.0f;
                            transform2.position.y += overlap.y / 2.0f;
                        }
                        else 
                        {
                            transform1.position.y += overlap.y / 2.0f;
                            transform2.position.y -= overlap.y / 2.0f;
                        }
                        transform1.velocity.y = 0;
                    }
                    
                    if (ent1->hasComponent<Comp::Moves>() && transform2.position.y > transform1.position.y)
                    {
                        auto& moves = ent1->getComponent<Comp::Moves>();
                        if (ent1->hasComponent<Comp::CoyoteTime>() && !moves.grounded)
                        {
                            ent1->PushEvent("ResetCoyoteTime");
                        }
                        moves.grounded = true;
                    }

                }
                else if (overlap.x < overlap.y) // resolve collision on the x axis
                {
                    if (bbox2.isStatic)
                    {
                        if (transform1.position.x < transform2.position.x) transform1.position.x -= overlap.x;
                        else transform1.position.x += overlap.x;
                        transform1.velocity.x = 0;
                    }
                    else
                    {
                        if (transform1.position.x <= transform2.position.x) 
                        {
                            transform1.position.x -= overlap.x / 2.0f;
                            transform2.position.x += overlap.x / 2.0f;
                        }
                        else 
                        {
                            transform1.position.x += overlap.x / 2.0f;
                            transform2.position.x -= overlap.x / 2.0f;
                        }
                        
                        transform1.velocity.x = 0;
                    }
                }        
            }

        }
        if (!collided)
        {
            if (ent1->hasComponent<Comp::CoyoteTime>() && ent1->hasComponent<Comp::Moves>())
            {
                if (ent1->getComponent<Comp::CoyoteTime>().expired) continue;
                ent1->getComponent<Comp::CoyoteTime>().coyoteTimer += m_engine.deltaTime();
                ent1->PushEvent("UpdateCoyoteTime");
            }
            else if (ent1->hasComponent<Comp::Moves>())
            {
                ent1->getComponent<Comp::Moves>().grounded = false;
            }
            
        }
    }
}