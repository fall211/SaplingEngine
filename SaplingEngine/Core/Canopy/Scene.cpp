//
//  Scene.cpp
//  SaplingEngine, Canopy Scene Manager
//  

#include "Scene.hpp"


Scene::Scene(Engine& engine) : m_engine(engine)
{
    m_entityManager = std::make_shared<EntityManager>();
}


void Scene::preUpdate()
{
    m_entityManager->update();
}

void Scene::postUpdate()
{
    Input::clean();
}

void Scene::enable()
{
    // set the window's event callback to our input system
    m_engine.getWindow().SetEventCallback([this](const sapp_event* e) { Input::update(e); });
}

void Scene::disable()
{
    Debug::log("disabling scene");
}

void Scene::sRender(EntityList& entities)
{
    for (const auto& e : entities)
    {
        if (e->hasComponent<Comp::Sprite>())
        {
            
            auto& cSprite = e->getComponent<Comp::Sprite>();
            auto& cTransform = e->getComponent<Comp::Transform>();

            if (cSprite.type == Comp::Sprite::Type::Animated)
            {
                // update the animation frame                
                if (m_engine.getCurrentFrame() % cSprite.animationSpeed == 0 )
                {
                    cSprite.currentFrame = (cSprite.currentFrame + 1) % cSprite.numFrames;
                }
                // m_engine.getWindow().draw_sprite(cSprite.texture, cTransform.position, cTransform.rotation, (int)cSprite.currentFrame);
                // return;
            }
            glm::f32 layer = static_cast<glm::f32>(cSprite.layer) / static_cast<glm::f32>(Comp::Sprite::Layer::Count);
            
            glm::vec2 pos = cTransform.position + cSprite.transformOffset;
            glm::vec3 scale = cTransform.scale * cSprite.scaleOffset;
            
            m_engine.getWindow().draw_sprite(cSprite.texture, pos, layer, cTransform.rotation, (int)cSprite.currentFrame, cSprite.color_override, scale, cTransform.pivot);
        }
    }
}