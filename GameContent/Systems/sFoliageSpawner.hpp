//
//
//
//

#pragma once

#include "Debug.hpp"
#include "EntityManager.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include <memory>

namespace System
{
    inline void FoliageSpawner(std::shared_ptr<EntityManager> entityManager)
    {
        std::vector<glm::vec2> positions = {
            {96, 254},
            {-24, 326},
            {-168, 326},
            {336, 374},
            {480, 374},
            {432, 134},
            {336, 38}
        };
        
        for (const auto& pos : positions)
        {
            auto tree = entityManager->addEntity({"tree", "static"});
            auto& treeTransform = tree->addComponent<Comp::Transform>(pos);
            treeTransform.pivot = Sprout::Pivot::BOTTOM_CENTER;
            tree->addComponent<Comp::Sprite>(AssetManager::getTexture("tree")).layer = Comp::Sprite::Layer::Background;
        }
    }
}