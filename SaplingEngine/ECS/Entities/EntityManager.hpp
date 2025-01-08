//
//  EntityManager.hpp
//  SaplingEngine
//

#pragma once

#include <string>
#include <map>
#include <memory>
#include <vector>

#include "Entity.hpp"
#include "Core/Canopy/Scene.hpp"

class Entity;

typedef std::vector<std::shared_ptr<Entity>> EntityList;
typedef std::map<std::string, EntityList> EntityMap;
typedef std::vector<std::string> TagList;

class EntityManager {
    EntityList m_entities;
    EntityList m_entitiesToAdd;
    EntityMap m_entityMap;
    size_t m_idCounter = 0;

public:
    EntityManager();
    void update();
    auto addEntity(const TagList& tags) -> std::shared_ptr<Entity>;
    auto getEntities() -> EntityList&;
    auto getEntities(const std::string& tag) -> EntityList&;
    
    template <typename T>
    auto getEntitiesByComponent() -> EntityList {
        EntityList entities;
        for (const auto& e : m_entities){
            if (e && e->hasComponent<T>()){
                entities.push_back(e);
            }
        }
        return entities;
    }
    
    void addTagToEntity(const std::shared_ptr<Entity>& entity, const std::string& tag);
    void removeTagFromEntity(const std::shared_ptr<Entity>& entity, const std::string& tag);
    void destroyEntity(const std::shared_ptr<Entity>& entity);
};

