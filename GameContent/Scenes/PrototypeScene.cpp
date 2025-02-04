//
//
//
//

#include "PrototypeScene.hpp"
#include "Component.hpp"
#include "Debug.hpp"
#include "Input/Input.hpp"
#include "cMathAnim.hpp"
#include "sBubbleVelDecay.hpp"
#include "sEnemySpawner.hpp"
#include <memory>




PrototypeScene::PrototypeScene(Engine& engine) : Scene(engine)
{
    init();
}

void PrototypeScene::init()
{
    sSpawn();
}

void PrototypeScene::update()
{
    //start of frame - run systems
    System::Gravity(m_entityManager->getEntities());
    System::EnemySpawner(m_entityManager->getEntities("spawner"), m_entityManager, m_engine.deltaTime());
    System::EnemyAI(m_entityManager->getEntities("enemy"), m_entityManager->getEntities("player").front(), m_entityManager->getEntities("map").front());
    System::WeaponUse(m_entityManager->getEntities("weapon"), m_entityManager, m_engine.deltaTime());
    System::BubbleVelDecay(m_entityManager->getEntities("projectile"));
    System::Lifetime(m_entityManager->getEntities(), m_engine.deltaTime());
    
    
    // mid frame - move entities
    sMoveCamera(m_entityManager->getEntities("player").front());
    System::Move(m_entityManager->getEntities("dynamic"), m_engine.deltaTime());
    System::PlayerController::Update(m_entityManager->getEntities("player").front(), m_entityManager->getEntities(), m_engine.deltaTime());

    
    // end of frame - resolve collisions, animate & render
    sResolveCollisions(m_entityManager->getEntities("dynamic"), m_entityManager->getEntities());
    System::MathAnim(m_entityManager->getEntities(), m_engine.getCurrentFrame() / 60.0f);
    auto entitiesWithSprite = m_entityManager->getEntitiesByComponent<Comp::Sprite>();
    sRender(entitiesWithSprite);
    
    // post frame - debug draws
    System::DrawTransforms(m_entityManager->getEntities());
}

void PrototypeScene::sSpawn()
{
    auto map = System::SpawnMap(std::string(ASSETS_PATH) + "map1.csv", *m_entityManager);

    auto player = m_entityManager->instantiatePrefab<Prefab::Player>();
    m_entityManager->instantiatePrefab<Prefab::Weapon>();
    
    System::FoliageSpawner(m_entityManager);
}

void PrototypeScene::sMoveCamera(const std::shared_ptr<Entity>& player)
{
    if (player->hasComponent<Comp::Transform>())
        {
            size_t camera_y_offset = 15;
            auto& target_pos = player->getComponent<Comp::Transform>().position;
            glm::vec2 camera_pos = m_engine.getWindow().getCameraPosition();
            
            glm::vec2 target = glm::vec2(target_pos.x, target_pos.y - camera_y_offset);
            
            float distance = glm::distance(target, camera_pos);
                
            float camera_speed = 0.25f * distance;
            camera_speed = glm::clamp(camera_speed, 0.1f, 10.0f);
            
            glm::vec2 direction = glm::normalize(target - camera_pos);
            camera_pos += direction * camera_speed;
            
            if (distance < 0.1f) {
                camera_pos = target;
            }
            
            m_engine.getWindow().setCameraPosition(camera_pos);
        }
}


void PrototypeScene::sResolveCollisions(const EntityList& dynamicEntities, const EntityList& entities)
{
    for (const auto& ent1 : dynamicEntities)
    {
        if (!ent1->hasComponentEnabled<Comp::BBox>()) continue;

        for (const auto& ent2: entities)
        {
            if (ent1->getId() == ent2->getId() 
                || !ent2->hasComponentEnabled<Comp::BBox>()
                // || ent2->getComponent<Comp::BBox>().collisionEventsEnabled
                || ((ent1->getComponent<Comp::BBox>().isTrigger
                && ent2->getComponent<Comp::BBox>().isTrigger) || ent2->getComponent<Comp::BBox>().interactWithTriggers)) continue;
            
            auto& bbox1 = ent1->getComponent<Comp::BBox>();
            auto& bbox2 = ent2->getComponent<Comp::BBox>();
                        
            glm::vec2 overlap = Physics2D::bBoxCollision(ent1, ent2);

            if (overlap.x != 0 || overlap.y != 0)
            {
                auto& transform1 = ent1->getComponent<Comp::Transform>();
                auto& transform2 = ent2->getComponent<Comp::Transform>();

                if (bbox1.interactWithTriggers && bbox2.isTrigger)
                {
                    if (bbox1.collidingWith.find(&bbox2) == bbox1.collidingWith.end())
                    {
                        // not in the set, so we just entered the trigger
                        bbox1.collidingWith.insert(&bbox2);
                        ent1->PushEvent("TriggerEnter", ent1, ent2);
                    }
                    else 
                    {
                        // we are still colliding with the trigger
                        ent1->PushEvent("TriggerStay", ent1, ent2);
                    }
                    continue;
                }
                if (bbox1.collisionEventsEnabled)
                {
                    if (bbox1.collidingWith.find(&bbox2) == bbox1.collidingWith.end())
                    {
                        // not in the set, so we just entered the collider
                        bbox1.collidingWith.insert(&bbox2);
                        ent1->PushEvent("CollisionEnter", ent1, ent2);
                    }
                    else 
                    {
                        // we are still colliding with the collider
                        ent1->PushEvent("CollisionStay", ent1, ent2);
                    }
                }
                if (bbox2.collisionEventsEnabled)
                {
                    if (bbox2.collidingWith.find(&bbox1) == bbox2.collidingWith.end())
                    {
                        // not in the set, so we just entered the collider
                        bbox2.collidingWith.insert(&bbox1);
                        ent2->PushEvent("CollisionEnter", ent2, ent1);
                    }
                    else 
                    {
                        // we are still colliding with the collider
                        ent2->PushEvent("CollisionStay", ent2, ent1);
                    }
                }


                if (overlap.x > overlap.y) // resolve collision on the y axis
                {
                    if (bbox2.isStatic)
                    {
                        if (transform1.position.y < transform2.position.y) 
                        {
                            transform1.position.y -= overlap.y;
                        }
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

                }
                else if (overlap.x < overlap.y) // resolve collision on the x axis
                {
                    if (bbox2.isStatic)
                    {
                        if (transform1.position.x < transform2.position.x) transform1.position.x -= overlap.x;
                        else transform1.position.x += overlap.x;

                        if (!ent1->hasComponent<Comp::PlayerController>())
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
                        if (!ent1->hasComponent<Comp::PlayerController>())
                        transform1.velocity.x = 0;
                    }
                }        
            }
            else if (!bbox1.collidingWith.empty())
            {
                if (bbox1.collidingWith.find(&bbox2) != bbox1.collidingWith.end())
                {
                    bbox1.collidingWith.erase(&bbox2);
                    if (bbox1.interactWithTriggers)
                    ent1->PushEvent("TriggerExit", ent1, ent2);
                    if (bbox1.collisionEventsEnabled)
                    ent1->PushEvent("CollisionExit", ent1, ent2);
                }
            }
        }
    }
}

