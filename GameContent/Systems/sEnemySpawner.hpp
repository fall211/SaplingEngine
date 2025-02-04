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
        for (auto& e : entities)
        {
            if (!e->hasComponent<Comp::EnemySpawner<Prefab::Enemy>>()) continue;

            auto& spawner = e->getComponent<Comp::EnemySpawner<Prefab::Enemy>>();
            spawner._time += dt;
            
            auto numEnemies = entManager->getEntities("enemy").size();
            if (spawner._time >= spawner.interval && numEnemies < 15)
            {
                auto enemy = spawner.spawn(entManager);
                enemy->getComponent<Comp::Transform>().position = e->getComponent<Comp::Transform>().position;
                spawner._time = 0;
            }
            
        }
    }
}