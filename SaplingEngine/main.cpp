//
//  main.cpp
//  SaplingEngine
//

#include "Engine.hpp"
#include <memory>

#include "PrototypeScene.hpp"

auto main() -> int {

    const auto engine = std::make_shared<Engine>();

    engine->addTexture("test", "Sprites/test.png");
    engine->addTexture("player", "Sprites/player.png");
    engine->addTexture("playerSheet", "Sprites/playerSheet.png", 2);
    engine->addTexture("obstacle", "Sprites/obstacle.png");

    engine->newScene<TestScene>("game");
    engine->changeScene("game");
    
    engine->newScene<PrototypeScene>("prototype");
    engine->changeScene("prototype");
    
    engine->run();

    return 0;
}
