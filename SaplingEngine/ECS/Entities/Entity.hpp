//
//  Entity.hpp
//  SaplingEngine
//

#pragma once

#include <string>
#include <tuple>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>

#include "Component.hpp"

class Component;


typedef std::vector<std::string> TagList;


class Entity
{
    private:
        TagList m_tags;
        size_t m_id = 0;
        bool m_active = true;
        std::unordered_map<std::type_index, std::shared_ptr<Comp::Component>> m_components;
    
        friend class EntityManager;
        Entity(TagList  tags, size_t id);
        
        /*
            * Adds a tag to the entity, only accessible by the EntityManager
            * @param tag The tag to add
        */
        void addTag(const std::string& tag);
        
        /*
            * Removes a tag from the entity, only accessible by the EntityManager
            * @param tag The tag to remove
        */
        void removeTag(const std::string& tag);
    
    public:
    
        /*
            * Gets the id of the entity
            * @return The id of the entity
        */
        auto getId() const -> size_t;
        
        /*
            * Gets the tags of the entity
            * @return The tags of the entity
        */
        auto getTags() -> const TagList&;
        
        /*
            * Checks if the entity is active
            * @return True if the entity is active
        */
        auto isActive() const -> bool;
        
        /*
            * Marks entity for destruction by the EntityManager
        */
        void destroy();
    
        /*
            * Adds a component to the entity
            * @tparam T The type of the component
            * @tparam Args The arguments to pass to the component constructor
            * @param args The arguments to pass to the component constructor
            * @return The added component
        */
        template <typename T, typename... Args>
        auto addComponent(Args&&... args) -> T& {
            std::shared_ptr<T> component = std::make_shared<T>(std::forward<Args>(args)...);
            m_components[typeid(T)] = std::move(component);
            return *static_cast<T*>(m_components[typeid(T)].get());
        }
    
        /*
            * Removes a component from the entity
            * @tparam T The type of the component
        */
        template <typename T>
        void removeComponent() {
            m_components.erase(typeid(T));
        }
    
        /*
            * Gets a component from the entity
            * @tparam T The type of the component
            * @return The component
        */
        template <typename T>
        auto getComponent() const -> T& {
            const auto it = m_components.find(typeid(T));
            return *dynamic_cast<T*>(it->second.get());
        }
    
        /*
            * Checks if the entity has a component
            * @tparam T The type of the component
            * @return True if the entity has the component
        */
        template <typename T>
        auto hasComponent() const -> bool {
            return m_components.find(typeid(T)) != m_components.end();
        }

};

