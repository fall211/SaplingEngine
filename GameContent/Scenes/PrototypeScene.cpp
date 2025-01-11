//
//
//
//

#include "PrototypeScene.hpp"


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
    System::Rotate(m_entityManager->getEntities(), m_engine.deltaTime());
    
    auto entitiesWithSprite = m_entityManager->getEntitiesByComponent<Comp::Sprite>();
    sRender(entitiesWithSprite);
}

void PrototypeScene::sSpawn()
{
    const auto e = m_entityManager->addEntity({"square", "dynamic"});
    e->addComponent<Comp::Transform>(glm::vec2(0, 0), glm::vec2(0, 0));
    e->addComponent<Comp::Sprite>(m_engine.getAssets()->getTexture("test"));
    e->addComponent<Comp::BBox>(64, 64);
    e->addComponent<Comp::Rotate>(1.0f);
}

