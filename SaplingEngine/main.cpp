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
    AssetManager::addTileSet("forest_tileset", "Sprites/tileset_forest.png");
    AssetManager::addTexture("weapon_basic", "Sprites/weapon_basic.png");
    AssetManager::addTexture("bubble_projectile", "Sprites/bubble_projectile.png");
    AssetManager::addTexture("hiisi", "Sprites/hiisi.png", 2);
    AssetManager::addTexture("fern", "Sprites/fern_1.png", 6);
    AssetManager::addTexture("tree", "Sprites/tree_dark.png");
    AssetManager::addTexture("debugCircle", "Sprites/debug.png");

    // then we add input actions and axes
    Input::makeAction("jump", {SAPP_KEYCODE_SPACE});
    Input::makeAxis("horizontal", SAPP_KEYCODE_D, SAPP_KEYCODE_A);
    Input::makeAxis("vertical", SAPP_KEYCODE_S, SAPP_KEYCODE_W);
    Input::makeAction("dash", {SAPP_KEYCODE_LEFT_SHIFT});
    Input::makeAction("changeScene", {SAPP_KEYCODE_C});
    Input::makeAction("pickup", {SAPP_KEYCODE_E});
    Input::makeAction("fire", {SAPP_KEYCODE_F});
    
    Input::makeAxis("moveX", SAPP_KEYCODE_LEFT, SAPP_KEYCODE_RIGHT);
    Input::makeAxis("moveY", SAPP_KEYCODE_UP, SAPP_KEYCODE_DOWN);
    
    // then we create the scenes our game uses
    // engine->newScene<TestScene>("game");    
    engine->newScene<PrototypeScene>("prototype");
    

    engine->changeScene("prototype");
    // engine->changeScene("game");
    
    // finally we run the engine, note that the engine will run until the window is closed
    // and nothing after this line will be executed until then
    engine->run();

    return 0;
}
