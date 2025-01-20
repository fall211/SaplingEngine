//
//  main.cpp
//  SaplingEngine
//

#include "Debug.hpp"
#include "Engine.hpp"
#include <memory>

#include "PrototypeScene.hpp"
#include "Seedbank/AssetManager.hpp"
#include "TestScene.hpp"

auto main() -> int 
{

    // first we initialize the engine
    const auto engine = std::make_shared<Engine>();

    // then we add the textures and other assets we want to use
    AssetManager::addTexture("test", "Sprites/test.png");
    AssetManager::addTexture("player", "Sprites/player.png");
    AssetManager::addTexture("playerSheet", "Sprites/playerSheet.png", 2);
    AssetManager::addTexture("obstacle", "Sprites/obstacle.png");
    
    AssetManager::addTexture("player_idle", "Sprites/player_idle.png", 10);
    AssetManager::addTexture("player_run", "Sprites/player_run.png", 8);

    // then we create the scenes our game uses
    engine->newScene<TestScene>("game");
    engine->changeScene("game");
    
    engine->newScene<PrototypeScene>("prototype");
    engine->changeScene("prototype");
    
    // finally we run the engine, note that the engine will run until the window is closed
    // and nothing after this line will be executed until then
    engine->run();

    return 0;
}
