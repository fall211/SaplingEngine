//
//  EntityManager.cpp
//  ECS_Engine
//
//  Created by Tuukka Virtanen on 1/6/24.
//

#include "EntityManager.hpp"

EntityManager::EntityManager() {
    m_entities = EntityList();
    m_entitiesToAdd = EntityList();
    m_entityMap = EntityMap();
};

void EntityManager::update(){
    // add new entities
    for (const auto& e : m_entitiesToAdd){
        m_entities.push_back(e);
        const auto& tags = e->getTags();
        for (const auto& tag : tags){
            m_entityMap[tag].push_back(e);
        }
    }
    m_entitiesToAdd.clear();

    // Collect all inactive entities
    std::vector<std::shared_ptr<Entity>> entitiesToDestroy;
    for (const auto& e: m_entities){
        if (!e->isActive()){
            entitiesToDestroy.push_back(e);
        }
    }

    for (const auto& e: entitiesToDestroy){
        destroyEntity(e);
    }
}

auto EntityManager::addEntity(const TagList& tags) -> std::shared_ptr<Entity>{
    auto entity = std::shared_ptr<Entity>(new Entity(tags, m_idCounter++));
    m_entitiesToAdd.push_back(entity);
    return entity;
}

auto EntityManager::getEntities() -> EntityList&{
    return m_entities;
}

auto EntityManager::getEntities(const std::string& tag) -> EntityList&{
    return m_entityMap[tag];
}

void EntityManager::addTagToEntity(const std::shared_ptr<Entity>& entity, const std::string& tag) {
    entity->addTag(tag);
    m_entityMap[tag].push_back(entity);
}

void EntityManager::removeTagFromEntity(const std::shared_ptr<Entity>& entity, const std::string& tag) {
    entity->removeTag(tag);
    auto& entityListWithTag = m_entityMap[tag];
    entityListWithTag.erase(std::remove(entityListWithTag.begin(), entityListWithTag.end(), entity), entityListWithTag.end());
}

void EntityManager::destroyEntity(const std::shared_ptr<Entity>& entity) {
    for (const auto& tag : entity->getTags()) {
        auto& entityListWithTag = m_entityMap[tag];
        entityListWithTag.erase(std::remove(entityListWithTag.begin(), entityListWithTag.end(), entity), entityListWithTag.end());
    }

    m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
}

