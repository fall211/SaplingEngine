//
// 
//
//

#pragma once

#include "Canopy/Scene.hpp"
#include "Engine.hpp"
#include "GameComponents.hpp"
#include "player.hpp"

class PrototypeScene : public Scene 
{
    private:
        int crap = 0;
    
    public:
            PrototypeScene(Engine& engine);
            ~PrototypeScene() = default;
    
            void init() override;
            void update() override;
    
            void sSpawn();
            void sMove(const EntityList& entities);
};