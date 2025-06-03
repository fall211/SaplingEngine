//
//
//
//

#pragma once

#include "Utility/Debug.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Component.hpp"

namespace System
{
    inline void SYSNAME(std::shared_ptr<EntityManager>& entityManager)
    {
        auto& entities = entityManager->getEntities();
        for (auto& e : entities)
        {

            
        }
    }
}