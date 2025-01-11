//
// 
//
//

#pragma once

#include "Canopy/Scene.hpp"
#include "Engine.hpp"
#include "sRotate.hpp"
#include "cRotate.hpp"

class PrototypeScene : public Scene {
private:
    int crap = 0;

public:
        PrototypeScene(Engine& engine);
        ~PrototypeScene() = default;

        void init() override;
        void update() override;

        void sSpawn();

};