//
//
//
//


#pragma once

#include "Entity.hpp"
#include "EntityManager.hpp"
#include "Input/Input.hpp"
#include <memory>
#include "Component.hpp"
#include "Input/Input.hpp"
#include "cPlayerController.hpp"
#include <glm/glm.hpp>
#include <cmath>

namespace System::PlayerController
{
    
    // void PlayerController(const std::shared_ptr<Entity>& player, EntityList& entities, const std::shared_ptr<Input>& input);
    
    void Update(const std::shared_ptr<Entity>& player, EntityList& entities, const std::shared_ptr<Input>& input, float dt);
    
    void CheckCollision(const std::shared_ptr<Entity>& player, EntityList& entities);
    
    void GatherInput(const std::shared_ptr<Entity>& player, const std::shared_ptr<Input>& input);
    
    void HandleJump(const std::shared_ptr<Entity>& player);
    
    void HandleDirection(const std::shared_ptr<Entity>& player, float dt);
    
    void HandleDash(const std::shared_ptr<Entity>& player);
    
    void HandleGravity(const std::shared_ptr<Entity>& player, float dt);
    
    void ApplyMovement(const std::shared_ptr<Entity>& player);
    
}