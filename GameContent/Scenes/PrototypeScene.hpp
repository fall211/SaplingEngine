//
// 
//
//

#pragma once

#include "Canopy/Scene.hpp"
#include "Physics.hpp"
#include "Engine.hpp"
#include "player.hpp"

#include "Component.hpp"
#include "Debug.hpp"
#include "EntityManager.hpp"

#include "GameComponents.hpp"
#include "GameSystems.hpp"

#include "glm/geometric.hpp"
#include <string>

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
            
            void sResolveCollisions(const EntityList& entities);
};