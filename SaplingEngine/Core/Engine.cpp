//
//  Engine.cpp
//  SaplingEngine
//


#include "Engine.hpp"


Engine::Engine()
{
    m_scenes = sceneMap();
    
    m_assets = std::make_shared<Assets>();
    m_assets->addTexture("test", "Sprites/test.png");
    m_assets->addTexture("player", "Sprites/player.png");
    m_assets->addTexture("playerSheet", "Sprites/playerSheet.png", 2);
    m_assets->addTexture("obstacle", "Sprites/obstacle.png");
    
    addScene("game");
    changeCurrentScene("game");


    Debug::log("init completed");
}

void Engine::main()
{
    m_window.SetUpdateFrameCallback([this](double dt) { this -> update(dt);});
    
    m_window.Run(); // nothing after this gets called
}

void Engine::update(double dt)
{
    m_deltaTime = dt;
    
    m_currentScene->update();
    m_currentFrame++;
}

void Engine::addScene(const std::string& name)
{
    std::shared_ptr<Scene> ptr = std::make_shared<GameScene>(*this);
    if (m_scenes.find(name) == m_scenes.end())
    {
        m_scenes[name] = ptr;
    }
    else
    {
        Debug::log("scene already exists");
    }
}

void Engine::changeCurrentScene(const std::string& name)
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

auto Engine::getAssets() const -> std::shared_ptr<Assets> 
{ 
    return m_assets; 
}

void Engine::setAssets(const std::shared_ptr<Assets>& newAssets)
{
    m_assets = newAssets; 
}

