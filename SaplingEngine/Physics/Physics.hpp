//
//  Physics.hpp
//  SaplingEngine, Twig Physics
//

#ifndef Physics_hpp
#define Physics_hpp

#include "glm/glm.hpp"
class Entity;

#include "SaplingEngine.hpp"

class Physics2D {
public:

    static auto bBoxCollision(const std::shared_ptr<Entity>& e0, const std::shared_ptr<Entity>& e1) -> glm::vec2;
    
    static auto bCircleCollision(const std::shared_ptr<Entity>& e0, const std::shared_ptr<Entity>& e1) -> glm::vec2;
    
    static auto bBoxCircleCollision(const std::shared_ptr<Entity>& eBox, const std::shared_ptr<Entity>& eCircle) -> glm::vec2;
};

#endif /* Physics_hpp */
