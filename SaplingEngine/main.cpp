//
//  main.cpp
//  ECS_Engine
//
//  Created by Tuukka Virtanen on 1/6/24.
//

#include "Core/Engine.hpp"
#include <memory>

auto main() -> int {

    const auto engine = std::make_shared<Engine>();
    engine->mainLoop();

    return 0;
}
