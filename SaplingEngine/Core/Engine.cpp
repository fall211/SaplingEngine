//
//  Engine.cpp
//  ECS_Engine
//
//  Created by Tuukka Virtanen on 1/6/24.
//

#include <SFML/Graphics.hpp>

#include "Debug.hpp"
#include "scenes/TestScene.hpp"
#include "Engine.hpp"


Engine::Engine() {
    m_scenes = sceneMap();
    
    assets = std::make_shared<Assets>();
    assets->addTexture("test", "resources/test.png");
    assets->addTexture("player", "resources/player.png");
    assets->addTexture("playerSheet", "resources/playerSheet.png");
    assets->addTexture("obstacle", "resources/obstacle.png");

    
    m_window.create(sf::VideoMode(1280, 720), "engine window");
    m_window.setFramerateLimit(0);
    m_currentFrame = 0;
    addScene("game", std::make_shared<GameScene>(*this));
    changeCurrentScene("game");
    addScene("menu", std::make_shared<MenuScene>(*this));
    changeCurrentScene("menu");
    addScene("collisionTest", std::make_shared<TestScene>(*this));
    changeCurrentScene("collisionTest");

    Debug::log("init completed");
}

void Engine::mainLoop(){
    const auto start = system_clock::now();
    auto previous = system_clock::now();

    while (m_window.isOpen()){
        calculateDeltaTime(start, previous);

        m_window.clear(sf::Color::White);
        
        m_currentScene->update();
        
        m_currentFrame++;
        m_window.display();
    }
}

void Engine::calculateDeltaTime(const time_point<system_clock>& start, time_point<system_clock>& previous) {
    const auto now = system_clock::now();
    const duration<float> elapsedTime = now - start;
    const duration<float> delta = now - previous;
    previous = now;

    m_deltaTime = delta.count();
    m_simTime = elapsedTime.count();
}


void Engine::addScene(const std::string& name, const std::shared_ptr<Scene>& ptr){
    if (m_scenes.find(name) == m_scenes.end()){
        m_scenes[name] = ptr;
    }
    else{
        Debug::log("scene already exists");
    }
}

void Engine::changeCurrentScene(const std::string& name){
    m_currentScene = getScene(name);
}

auto Engine::getScene(const std::string& name) -> std::shared_ptr<Scene>{
    return m_scenes[name];
}

auto Engine::getCurrentScene() -> std::shared_ptr<Scene>&{
    return m_currentScene;
}