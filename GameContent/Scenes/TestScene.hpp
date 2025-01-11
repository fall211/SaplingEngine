//
//
//
//

#pragma once

#include "Canopy/Scene.hpp"



class TestScene final : public Scene 
{
    private:
        void sSpawnPlayer() const;
        void sPlayerGravity(const std::shared_ptr<Entity>& player) ;
        void sPlayerController(const std::shared_ptr<Entity>& player) const;
        void sMove(const EntityList& entities) ;
        
        void sSpawnSquare();
        void sMoveSquare(const std::shared_ptr<Entity>& square) const;
        
        float m_obstacleSpawnTimer = 0;
        void sSceneTime();
        void sObstacleSpawner();
        static void sDeleteOffScreen(const EntityList& entities);
        static void sCollisionHandler(const std::shared_ptr<Entity>& player, const EntityList& obstacles) ;
        
        void sMoveCamera();
    
    public:
        explicit TestScene(Engine& engine);
        void init() override;
        void update() override;
};
