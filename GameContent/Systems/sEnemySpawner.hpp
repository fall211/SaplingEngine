//
//
//
//

#pragma once

#include "Debug.hpp"
#include "EntityManager.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "cEnemySpawner.hpp"
#include <memory>
#include "enemy.hpp"

namespace System
{
    inline void EnemySpawner(EntityList& entities, const std::shared_ptr<EntityManager>& entManager, float dt)
    {
        std::shuffle(entities.begin(), entities.end(), std::mt19937(std::random_device()()));
        
        for (auto& e : entities)
        {
            if (!e->hasComponent<Comp::EnemySpawner<Prefab::ForestSpirit>>()) continue;

            auto& spawner = e->getComponent<Comp::EnemySpawner<Prefab::ForestSpirit>>();
            spawner._time += dt;
            
            auto numEnemies = entManager->getEntities("enemy").size();
            if (spawner._time >= spawner.interval && numEnemies < 10)
            {
                auto enemy = spawner.spawn(entManager);
                enemy->getComponent<Comp::Transform>().position = e->getComponent<Comp::Transform>().position;
                spawner._time = 0;
                
                entManager->getSpatialGrid().updateEntity(enemy);
            }
            
        }
    }
}