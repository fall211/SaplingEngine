//
//  main.cpp
//  SaplingEngine
//

#include "Core/Engine.hpp"
#include <memory>

auto main() -> int {

    const auto engine = std::make_shared<Engine>();
    engine->mainLoop();

    return 0;
}
