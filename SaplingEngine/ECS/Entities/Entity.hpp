//
//  Entity.hpp
//  ECS_Engine
//
//  Created by Tuukka Virtanen on 1/6/24.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <string>
#include <tuple>
#include <unordered_map>
#include <typeindex>

class Component;

#include "SaplingEngine.hpp"

typedef std::vector<std::string> TagList;


class Entity {
    TagList m_tags;
    const size_t m_id = 0;
    bool m_active = true;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components;

    friend class EntityManager;
    Entity(TagList  tags, size_t id);
    void addTag(const std::string& tag);
    void removeTag(const std::string& tag);
    
public:
    [[nodiscard]] auto getId() const -> size_t;
    auto getTags() -> const TagList&;
    [[nodiscard]] auto isActive() const -> bool;
    void destroy();

    
    template <typename T, typename... Args>
    auto addComponent(Args&&... args) -> T& {
        std::shared_ptr<T> component = std::make_shared<T>(std::forward<Args>(args)...);
        m_components[typeid(T)] = std::move(component);
        return *static_cast<T*>(m_components[typeid(T)].get());
    }

    template <typename T>
    void removeComponent() {
        m_components.erase(typeid(T));
    }

    template <typename T>
    auto getComponent() const -> T& {
        const auto it = m_components.find(typeid(T));
        return *dynamic_cast<T*>(it->second.get());
    }

    template <typename T>
    [[nodiscard]] auto hasComponent() const -> bool {
        return m_components.find(typeid(T)) != m_components.end();
    }

};

#endif /* Entity_hpp */
