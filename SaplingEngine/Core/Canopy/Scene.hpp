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
#include "Input/Input.hpp"

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


class Scene
{
    protected:
        std::shared_ptr<EntityManager> m_entityManager; // the scene's entity manager
        std::shared_ptr<Input> m_input; // the scene's input manager
        Engine& m_engine; // the engine that the scene is running on
    
    
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
        
        /*
            * Called when the scene is switched to
        */
        void enable();
        
        /*
            * Called when the scene is switched from
        */
        void disable();
        
        /*
            * Called before each update loop, reserved for updates that are necessary for all scenes
        */
        void preUpdate();
        
        /*
            * Called after each update loop, reserved for updates that are necessary for all scenes
        */
        void postUpdate();
        
        
        auto getInput() -> std::shared_ptr<Input>;
};


