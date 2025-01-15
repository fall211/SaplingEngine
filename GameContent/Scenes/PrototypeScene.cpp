//
//
//
//

#include "PrototypeScene.hpp"
#include "Debug.hpp"
#include "cLowestY.hpp"
#include "cRotate.hpp"

PrototypeScene::PrototypeScene(Engine& engine) : Scene(engine)
{
    init();
}

void PrototypeScene::init()
{
    m_input->makeAction("space", {SAPP_KEYCODE_SPACE});
    
    sSpawn();
}

void PrototypeScene::update()
{
    sMove(m_entityManager->getEntities());

    
    
    auto entitiesWithSprite = m_entityManager->getEntitiesByComponent<Comp::Sprite>();
    sRender(entitiesWithSprite);
}

void PrototypeScene::sSpawn()
{
    auto player = m_entityManager->instantiatePrefab<Prefab::Player>();
}

void PrototypeScene::sMove(const EntityList& entities)
{
    for (const auto& entity : entities)
    {
        if (entity->hasComponent<Comp::Gravity>() && entity->hasTag("gravityAffected"))
        {
            entity->getComponent<Comp::Transform>().velocity.y += entity->getComponent<Comp::Gravity>().gravity * m_engine.deltaTime();
        }
        
        if (entity->hasComponent<Comp::LowestY>())
        {
            if (entity->getComponent<Comp::Transform>().position.y > entity->getComponent<Comp::LowestY>().y_level)
            {
                entity->getComponent<Comp::Transform>().position.y = entity->getComponent<Comp::LowestY>().y_level;
                entity->getComponent<Comp::Transform>().velocity.y = 0;
            }
        }
        
        entity->getComponent<Comp::Transform>().position += entity->getComponent<Comp::Transform>().velocity * m_engine.deltaTime();
    }
    
}
