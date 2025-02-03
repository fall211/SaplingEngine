//
//
//
//

#include "PrototypeScene.hpp"
#include "Component.hpp"
#include "Debug.hpp"
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
    System::Gravity(m_entityManager->getEntities());
    System::EnemyAI(m_entityManager->getEntities("enemy"), m_entityManager->getEntities("player").front());
    System::Move(m_entityManager->getEntities("dynamic"), m_engine.deltaTime());
    System::PlayerController::Update(m_entityManager->getEntities("player").front(), m_entityManager->getEntities(), m_engine.deltaTime());

    sResolveCollisions(m_entityManager->getEntities());
    
    sMoveCamera(m_entityManager->getEntities("player").front());
    auto entitiesWithSprite = m_entityManager->getEntitiesByComponent<Comp::Sprite>();
    sRender(entitiesWithSprite);
}

void PrototypeScene::sSpawn()
{
    auto map = System::SpawnMap(std::string(ASSETS_PATH) + "map1.csv", *m_entityManager);


    auto player = m_entityManager->instantiatePrefab<Prefab::Player>();
    m_entityManager->instantiatePrefab<Prefab::Weapon>();


    m_entityManager->instantiatePrefab<Prefab::Enemy>();

}
void PrototypeScene::sMoveCamera(const std::shared_ptr<Entity>& player)
{

    if (player->hasComponent<Comp::Transform>())
    {
        size_t camera_y_offset = 15;
        auto& transform = player->getComponent<Comp::Transform>();
        m_engine.getWindow().setCameraPosition({transform.position.x, transform.position.y - camera_y_offset});
    }
}

void PrototypeScene::sResolveCollisions(const EntityList& entities)
{
    for (const auto& ent1 : entities)
    {
        if (!ent1->hasComponentEnabled<Comp::BBox>()) continue;
        if (ent1->getComponent<Comp::BBox>().isStatic) continue;
        for (const auto& ent2: entities)
        {
            if (ent1->getId() == ent2->getId() 
                || !ent2->hasComponentEnabled<Comp::BBox>()
                || (ent1->getComponent<Comp::BBox>().isTrigger
                && ent2->getComponent<Comp::BBox>().isTrigger)) continue;

            glm::vec2 overlap = Physics2D::bBoxCollision(ent1, ent2);

            auto& bbox1 = ent1->getComponent<Comp::BBox>();
            auto& bbox2 = ent2->getComponent<Comp::BBox>();
            
            if (overlap.x != 0 || overlap.y != 0)
            {
                auto& transform1 = ent1->getComponent<Comp::Transform>();
                auto& transform2 = ent2->getComponent<Comp::Transform>();

                if (bbox1.interactWithTriggers && bbox2.isTrigger)
                {
                    if (std::find(bbox1.collidingWith.begin(), bbox1.collidingWith.end(), &bbox2) == bbox1.collidingWith.end())
                    {
                        // not in the list, so we just entered the trigger
                        bbox1.collidingWith.push_back(&bbox2);
                        ent1->PushEvent("TriggerEnter", ent1, ent2);
                    }
                    else 
                    {
                        // we are still colliding with the trigger
                        ent1->PushEvent("TriggerStay", ent1, ent2);
                    }
                    continue;
                }
                if (bbox2.interactWithTriggers && bbox1.isTrigger)
                {
                    // prevent double triggering
                    continue;
                }

                if (overlap.x > overlap.y) // resolve collision on the y axis
                {
                    if (bbox2.isStatic)
                    {
                        if (transform1.position.y < transform2.position.y) transform1.position.y -= overlap.y;
                        else transform1.position.y += overlap.y;

                        if (!ent1->hasComponent<Comp::PlayerController>())
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
                        if (!ent1->hasComponent<Comp::PlayerController>())
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
                if (std::find(bbox1.collidingWith.begin(), bbox1.collidingWith.end(), &bbox2) != bbox1.collidingWith.end())
                {
                    bbox1.collidingWith.erase(std::remove(bbox1.collidingWith.begin(), bbox1.collidingWith.end(), &bbox2), bbox1.collidingWith.end());
                    ent1->PushEvent("TriggerExit", ent1, ent2);
                }
            }
        }
    }
}