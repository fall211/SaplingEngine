//
//  Engine.cpp
//  SaplingEngine
//


#include "Engine.hpp"
#include "Seedbank/AssetManager.hpp"
#include <memory>


Engine::Engine()
{
    m_scenes = sceneMap();
    AssetManager::getInstance();

    Debug::log("init completed");
}

void Engine::run()
{
    m_window.SetUpdateFrameCallback([this](double dt) { this -> update(dt);});
    
    m_window.Run(); // nothing after this gets called
}

void Engine::update(double dt)
{
    m_deltaTime = dt;
    
    if (!m_currentScene)
    {
        Debug::log("ERR: No scene set");
        return;
    }
    
    m_currentScene->preUpdate();
    m_currentScene->update();
    m_currentScene->postUpdate();
    m_currentFrame++;
}

void Engine::makeScene(const std::string& name, std::shared_ptr<Scene> ptr)
{
    if (m_scenes.find(name) == m_scenes.end())
    {
        m_scenes[name] = std::move(ptr);
    }
    else
    {
        Debug::log("scene already exists");
    }
}

void Engine::changeScene(const std::string& name)
{
    if (m_currentScene)
    {
        m_currentScene->disable();
    }
    m_currentScene = getScene(name);
    m_currentScene->enable();
}

auto Engine::getScene(const std::string& name) -> std::shared_ptr<Scene>
{
    return m_scenes[name];
}

auto Engine::getCurrentScene() -> std::shared_ptr<Scene>&
{
    return m_currentScene;
}



