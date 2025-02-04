//
//
//
//

#pragma once

#include <memory>
#include <utility>

#include "Component.hpp"
#include "Entity.hpp"
#include "Debug.hpp"
#include "EntityManager.hpp"

namespace Comp
{
    template<typename PrefabType>
    class EnemySpawner : public Component 
    {
        public:
            float interval;
            float _time = 0;
            
            explicit EnemySpawner(Inst inst, float interval = 7.5f) 
            :   Component(std::move(inst)), 
                interval(interval)
            {}
            
            auto spawn(const std::shared_ptr<EntityManager>& entManager) -> std::shared_ptr<Entity>
            {
                return entManager->instantiatePrefab<PrefabType>();
            }
    };
}