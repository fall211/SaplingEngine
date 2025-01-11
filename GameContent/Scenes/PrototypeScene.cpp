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
    Systems::sRotate(m_entityManager->getEntities(), m_engine.deltaTime());
    
    auto entitiesWithSprite = m_entityManager->getEntitiesByComponent<CSprite>();
    sRender(entitiesWithSprite);
}

void PrototypeScene::sSpawn()
{
    const auto e = m_entityManager->addEntity({"square", "dynamic"});
    e->addComponent<CTransform>(glm::vec2(0, 0), glm::vec2(0, 0));
    e->addComponent<CSprite>(m_engine.getAssets()->getTexture("test"));
    e->addComponent<CBBox>(64, 64);
    e->addComponent<CRotate>(1.0f);
}

