//
//  Scene.hpp
//  SaplingEngine, Canopy Scene Manager
//

#pragma once

#include "Physics/Physics.hpp"

#include "Engine.hpp"
#include "Debug.hpp"

#include "EntityManager.hpp"
#include "Entity.hpp"
#include "Component.hpp"

#include <cstdio>
#include <memory>
#include <random>
#include <cstddef>
#include <string>

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
    
    /*
        * Called when the scene is first created.
        * Override this function to initialize the scene
    */
    virtual void init() = 0;
    
    /*
        * Called every frame.
        * Override this function to update the scene
    */
    virtual void update() = 0;
    
    /*
        * Called every frame to render entities
        * Override this function to implement custom rendering, by default it renders all entities with a sprite component (static and animated)
        * @param entities The list of entities to render
    */
    virtual void sRender(EntityList& entities);
    
    void enable();
    void disable();
    
    /*
        * Called before each update loop, reserved for updates that are necessary for all scenes
    */
    void preUpdate();
    
    /*
        * Called after each update loop, reserved for updates that are necessary for all scenes
    */
    void postUpdate();
};


class TestScene final : public Scene {
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
