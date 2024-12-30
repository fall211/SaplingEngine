//
//  Scene.hpp
//  SaplingEngine, Canopy Scene Manager
//

#pragma once

#include "SaplingEngine.hpp"

#include <cstdio>
#include <random>

class Entity;
class Input;
class EntityManager;
class Engine;



typedef std::vector<std::shared_ptr<Entity>> EntityList;


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
    
    void enable();
    void disable();
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
