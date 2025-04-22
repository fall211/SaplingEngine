//
//  Scene.cpp
//  SaplingEngine, Canopy Scene Manager
//  

#include "Scene.hpp"
#include "Audio/AudioEngine.hpp"
#include "Component.hpp"
#include "Sprout.hpp"


Scene::Scene(Engine& engine) : m_engine(engine)
{
    m_entityManager = std::make_shared<EntityManager>();
}


void Scene::preUpdate()
{
    m_entityManager->update();
    AudioEngine::getInstance()->update();
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
    float dt = m_engine.deltaTime();
    for (const auto& e : entities)
    {
        if (e->hasComponent<Comp::Sprite>())
        {
            
            auto& cSprite = e->getComponent<Comp::Sprite>();

            if (cSprite.type == Comp::Sprite::Type::Animated)
            {
                // update the animation frame based on delta time
                cSprite.animationTime += dt;
                if (cSprite.animationTime >= (1.0f / cSprite.animationSpeed))
                {
                    cSprite.currentFrame = (cSprite.currentFrame + 1) % cSprite.numFrames;
                    cSprite.animationTime = 0.0f;
                }
            }
            glm::f32 layer = 0.0f;
            glm::vec2 pos = glm::vec2(0.0f);
            glm::vec3 scale = glm::vec3(1.0f);
            glm::f32 rotation = 0.0f;
            Sprout::Pivot pivot = Sprout::Pivot::CENTER;
            bool worldSpace = true;
            
            if (e->hasComponent<Comp::Transform>())
            {
                auto& cTransform = e->getComponent<Comp::Transform>();
                layer = static_cast<glm::f32>(cSprite.layer) / static_cast<glm::f32>(Comp::Sprite::Layer::Count);
                pos = cTransform.position + cSprite.transformOffset;
                scale = cTransform.scale * cSprite.scaleOffset;
                rotation = cTransform.rotation;
                pivot = cTransform.pivot;
            }
            else if (e->hasComponent<Comp::GUITransform>())
            {
                auto& cUITransform = e->getComponent<Comp::GUITransform>();
                layer = 1.0f;
                worldSpace = false;
                pos = cUITransform.screenPosition;
                pivot = cUITransform.pivot;
                scale = cUITransform.scale;
            }
            
            m_engine.getWindow().draw_sprite(cSprite.texture, pos, layer, rotation, (int)cSprite.currentFrame, cSprite.color_override, scale, pivot, worldSpace);
            
            if (cSprite.colorOverrideTime > 0)
            {
                cSprite.colorOverrideTime -= dt;
                if (cSprite.colorOverrideTime <= 0)
                {
                    cSprite.color_override = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
                    cSprite.colorOverrideTime = 0;
                }
            }
        }
    }
}