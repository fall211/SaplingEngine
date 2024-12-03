//
//  Scene.hpp
//  ECS_Engine
//
//  Created by Tuukka Virtanen on 1/25/24.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <cstdio>
#include <random>
#include "SaplingEngine.hpp"

class Engine;

class Scene {
protected:
    std::shared_ptr<EntityManager> m_entityManager;
    std::shared_ptr<Input> m_input;
    Engine& m_engine;


public:
    explicit Scene(Engine& engine);
    virtual ~Scene()= default;
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void sRender(EntityList& entities);
};



class GameScene final : public Scene {
    void sSpawnPlayer() const;
    void sPlayerGravity(const std::shared_ptr<Entity>& player) ;
    void sPlayerController(const std::shared_ptr<Entity>& player) const;
    void sMove(const EntityList& entities) ;
    
    float m_obstacleSpawnTimer = 0;
    void sSceneTime();
    void sObstacleSpawner();
    static void sDeleteOffScreen(const EntityList& entities);
    static void sCollisionHandler(const std::shared_ptr<Entity>& player, const EntityList& obstacles) ;
    
public:
    explicit GameScene(Engine& engine);
    void init() override;
    void update() override;
};

class MenuScene final : public Scene {
    void sTest() const ;

public:
    explicit MenuScene(Engine& engine);
    void init() override;
    void update() override;
};

#endif /* Scene_hpp */
