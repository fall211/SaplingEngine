//
//
//
//

#include "PrototypeScene.hpp"
#include "cPlayerController.hpp"
#include "sPlayerController.hpp"



PrototypeScene::PrototypeScene(Engine& engine) : Scene(engine)
{
    init();
}

void PrototypeScene::init()
{
    m_input->makeAction("jump", {SAPP_KEYCODE_SPACE});
    m_input->makeAxis("horizontal", SAPP_KEYCODE_D, SAPP_KEYCODE_A);
    m_input->makeAxis("vertical", SAPP_KEYCODE_S, SAPP_KEYCODE_W);
    m_input->makeAction("dash", {SAPP_KEYCODE_LEFT_SHIFT});
    
    sSpawn();
}

void PrototypeScene::update()
{
    
    
    System::Gravity(m_entityManager->getEntities("gravityaffected"));
    System::Move(m_entityManager->getEntities(), m_engine.deltaTime());
    
    System::PlayerController::Update(m_entityManager->getEntities("player").front(), m_entityManager->getEntities(), m_input, m_engine.deltaTime());

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
    
    // auto obstacle = m_entityManager->addEntity({"static", "obstacle"});
    // obstacle->addComponent<Comp::Transform>(glm::vec2(-150, 150), glm::vec2(0, 0));
    // obstacle->addComponent<Comp::BBox>(1000, 10);
}


void PrototypeScene::sResolveCollisions(const EntityList& entities)
{
    for (const auto& ent1 : entities)
    {
        if (!ent1->hasComponent<Comp::BBox>()) continue;
        for (const auto& ent2: entities)
        {
            if (ent1->getId() == ent2->getId() || !ent2->hasComponent<Comp::BBox>()) continue;
            
            glm::vec2 overlap = Physics2D::bBoxCollision(ent1, ent2);
            
            if (overlap.x != 0 || overlap.y != 0)
            {
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

        }
    }
}