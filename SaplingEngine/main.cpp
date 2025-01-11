//
//  main.cpp
//  SaplingEngine
//

#include "Engine.hpp"
#include <memory>

#include "PrototypeScene.hpp"
#include "TestScene.hpp"


auto main() -> int 
{

    // first we initialize the engine
    const auto engine = std::make_shared<Engine>();

    // then we add the textures and other assets we want to use
    engine->addTexture("test", "Sprites/test.png");
    engine->addTexture("player", "Sprites/player.png");
    engine->addTexture("playerSheet", "Sprites/playerSheet.png", 2);
    engine->addTexture("obstacle", "Sprites/obstacle.png");

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
