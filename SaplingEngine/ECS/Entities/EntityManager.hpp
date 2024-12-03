//
//  EntityManager.hpp
//  ECS_Engine
//
//  Created by Tuukka Virtanen on 1/6/24.
//

#ifndef EntityManager_hpp
#define EntityManager_hpp

#include <string>
#include <map>

#include "Entity.hpp"

typedef std::vector<std::shared_ptr<Entity>> EntityList;
typedef std::map<std::string, EntityList> EntityMap;

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
    
    void addTagToEntity(const std::shared_ptr<Entity>& entity, const std::string& tag);
    void removeTagFromEntity(const std::shared_ptr<Entity>& entity, const std::string& tag);
    void destroyEntity(const std::shared_ptr<Entity>& entity);
};

#endif /* EntityManager_hpp */
