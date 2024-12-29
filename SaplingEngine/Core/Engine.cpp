//
//  Engine.cpp
//  SaplingEngine
//


#include "Engine.hpp"
#include "Core/Debug.hpp"

Engine::Engine() {
    m_scenes = sceneMap();
    
    // m_assets = std::make_shared<Assets>();
    // m_assets->addTexture("test", "../GameContent/Assets/Sprites/test.png");
    // m_assets->addTexture("player", "../GameContent/Assets/Sprites/player.png");
    // m_assets->addTexture("playerSheet", "../GameContent/Assets/Sprites/playerSheet.png");
    // m_assets->addTexture("obstacle", "../GameContent/Assets/Sprites/obstacle.png");
    
    addScene("game", std::make_shared<GameScene>(*this));
    changeCurrentScene("game");
    addScene("menu", std::make_shared<MenuScene>(*this));
    changeCurrentScene("menu");

    Debug::log("init completed");
}

void Engine::main(){
    const auto start = system_clock::now();
    auto previous = system_clock::now();

    m_window.SetUpdateFrameCallback([this](double dt) { this -> update(dt);});
    
    m_window.Run(); // nothing after this gets called
}

void Engine::update(double dt){

    m_currentScene->update();
    m_currentFrame++;
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

auto Engine::getAssets() const -> std::shared_ptr<Assets> { 
    return m_assets; 
}

void Engine::setAssets(const std::shared_ptr<Assets>& newAssets) {
    m_assets = newAssets; 
}

