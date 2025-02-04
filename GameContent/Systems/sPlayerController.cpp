//
//
//
//



#include "sPlayerController.hpp"
#include "Component.hpp"
#include "Debug.hpp"
#include "Sprout.hpp"
#include "glm/geometric.hpp"
#include <climits>
#include <memory>
#include <unordered_set>


namespace System::PlayerController
{    
    void Update(const std::shared_ptr<Entity>& player, EntityList& entities, float dt)
    {
        if (!player->hasComponent<Comp::PlayerController>() || 
            !player->hasComponent<Comp::Transform>() ||
            !player->hasComponent<Comp::BBox>()
        ) return;
        
        auto& controller = player->getComponent<Comp::PlayerController>();
        
        controller._time += dt;
        
        GatherInput(player);
        CheckCollision(player, entities);
        HandleJump(player);
        HandleDirection(player, dt);
        HandleDash(player);
        HandleGravity(player, dt);
        ApplyMovement(player);
        
        // move player to 0,0 if they fall off the map
        if (player->getComponent<Comp::Transform>().position.y > 1000)
        {
            glm::vec2 delta = glm::vec2(0, 0) - player->getComponent<Comp::Transform>().position;
            player->getComponent<Comp::Transform>().position = glm::vec2(0, 0);
            Sprout::Window::instance->translateCamera(delta.x, delta.y);
        }
    }
    
    void GatherInput(const std::shared_ptr<Entity>& player)
    {
        if (!player->hasComponent<Comp::PlayerController>()) return;
        
        auto& controller = player->getComponent<Comp::PlayerController>();
        
        controller.frameInput.jumpDown = Input::isActionDown("jump");
        controller.frameInput.jumpHeld = Input::isAction("jump");
        controller.frameInput.move = glm::vec2(Input::getAxis("horizontal"), Input::getAxis("vertical"));
        
        // dash
        controller.frameInput.dashDown = Input::isActionDown("dash") || Input::getMouseDown(Input::MouseButton::RIGHT);
        controller.frameInput.dashDirection = glm::vec2(Input::getAxis("horizontal"), Input::getAxis("vertical"));
        
        
        if (controller.frameInput.jumpDown)
        {
            controller.jumpToConsume = true;
            controller.timeJumpWasPressed = controller._time;
        }
        
        if (Input::isActionDown("pickup"))
        {
            player->PushEvent("PickupInput", player);
        }
    }
    
    void CheckCollision(const std::shared_ptr<Entity> &player, EntityList &entities)
        {
            if (!player->hasComponent<Comp::PlayerController>() || 
                !player->hasComponent<Comp::Transform>() ||
                !player->hasComponent<Comp::BBox>()
            ) return;

            auto& controller = player->getComponent<Comp::PlayerController>();
            auto& transform = player->getComponent<Comp::Transform>();

            bool groundHit = false;
            bool ceilingHit = false;

            std::shared_ptr<Entity> closestEntity = nullptr;
            int smallestXDiff = INT_MAX;
            
            for (auto& other : entities)
            {
                if (other->getId() == player->getId()) continue;

                if (!other->hasComponent<Comp::BBox>() || 
                    !other->hasComponent<Comp::Transform>() ||
                    other->getComponent<Comp::BBox>().isTrigger
                ) continue;
                
                glm::vec2 overlap = Physics2D::bBoxCollision(player, other);
                if (overlap.x != 0 || overlap.y != 0)
                {                    
                    auto xDiff = abs(transform.position.x - other->getComponent<Comp::Transform>().position.x);
                    if (xDiff < smallestXDiff)
                    {
                        smallestXDiff = xDiff;
                        closestEntity = other;
                    }
                }
            }
            
            if (smallestXDiff != INT_MAX)
            {
                auto& other_transform = closestEntity->getComponent<Comp::Transform>();
                
                if (smallestXDiff > 12)
                {
                    groundHit = false;
                    ceilingHit = false;
                }
                else if (transform.position.y < other_transform.position.y) groundHit = true;
                else if (transform.position.y > other_transform.position.y) ceilingHit = true;

            }
            
            if (ceilingHit) 
            {
                controller.frameVelocity.y = fmax(0, controller.frameVelocity.y);
            }

            if (!controller.grounded && groundHit)
            {
                controller.grounded = true;
                controller.hasDash = true;
                controller.coyoteUsable = true;
                controller.bufferedJumpUsable = true;
                controller.endedJumpEarly = false;
            }
            else if (controller.grounded && !groundHit)
            {
                controller.grounded = false;
                controller.frameLeftGround = controller._time;        
            }
        }
    
    void HandleJump(const std::shared_ptr<Entity>& player)
    {
        if (!player->hasComponent<Comp::PlayerController>() || 
            !player->hasComponent<Comp::Transform>()
        ) return;
        
        auto& controller = player->getComponent<Comp::PlayerController>();
        auto& transform = player->getComponent<Comp::Transform>();
        
        if (!controller.endedJumpEarly && !controller.grounded && !controller.frameInput.jumpHeld && transform.velocity.y < 0)
        {
            controller.endedJumpEarly = true;
        }
        
        if (!controller.jumpToConsume && !controller.HasBufferedJump()) return;
        
        if (controller.grounded || controller.CanUseCoyote())
        {
            player->PushEvent("Jump", player);
        }
        
        controller.jumpToConsume = false;
    }
    
    void HandleDirection(const std::shared_ptr<Entity>& player, float dt)
    {
        if (!player->hasComponent<Comp::PlayerController>()
        ) return;
        
        auto& controller = player->getComponent<Comp::PlayerController>();
        
        auto moveTowards = [](float current, float target, float maxDelta) -> float {
            if (std::abs(target - current) <= maxDelta) {
                return target;
            }
            return current + glm::sign(target - current) * maxDelta;
        };
        
        if (controller.frameInput.move.x == 0) 
        {
            auto deceleration = controller.grounded ? controller.groundDeceleration : controller.airDeceleration;
            controller.frameVelocity.x = moveTowards(controller.frameVelocity.x, 0, deceleration * dt);
            if (player->hasComponent<Comp::Sprite>())
            {
                auto& sprite = player->getComponent<Comp::Sprite>();
                sprite.texture = AssetManager::getTexture("player_idle");
            }
        }
        else if (controller.IsDashing())
        {
            controller.frameVelocity.x = controller.frameVelocity.x;
        }
        else
        {
            controller.frameVelocity.x = moveTowards(controller.frameVelocity.x, controller.frameInput.move.x * controller.maxSpeed, controller.acceleration * dt);
            controller.flipSpriteX();
            if (player->hasComponent<Comp::Sprite>())
            {
                auto& sprite = player->getComponent<Comp::Sprite>();
                sprite.texture = AssetManager::getTexture("player_run");
            }
        }
    }
    
    void HandleDash(const std::shared_ptr<Entity> &player)
    {
        if (!player->hasComponent<Comp::PlayerController>()
        ) return;
        
        auto& controller = player->getComponent<Comp::PlayerController>();
        
        if (controller.frameInput.dashDown && controller.frameInput.dashDirection != glm::vec2(0, 0) && controller.CanUseDash())
        {
            controller.frameInput.dashDirection = glm::normalize(controller.frameInput.dashDirection);
            player->PushEvent("Dash", player);
        }
    }
    
    void HandleGravity(const std::shared_ptr<Entity> &player, float dt)
    {
        if (!player->hasComponent<Comp::PlayerController>()
        ) return;
        
        auto moveTowards = [](float current, float target, float maxDelta) -> float {
            if (std::abs(target - current) <= maxDelta) {
                return target;
            }
            return current + glm::sign(target - current) * maxDelta;
        };
        
        auto& controller = player->getComponent<Comp::PlayerController>();

        if (controller.grounded && controller.frameVelocity.y >= 0.0f)
        {
            controller.frameVelocity.y = controller.groundingForce;
        }
        else if (controller.IsDashing())
        {
            controller.frameVelocity.y = controller.frameVelocity.y;
        }
        else
        {
            auto inAirGravity = controller.fallAcceleration;
            if (controller.endedJumpEarly  && controller.frameVelocity.y <= 0)
            {
                inAirGravity *= controller.jumpEndEarlyGravityModifier;
            }
            
            controller.frameVelocity.y = moveTowards(controller.frameVelocity.y, controller.maxFallSpeed, inAirGravity * dt);
        }
    }
    
    void ApplyMovement(const std::shared_ptr<Entity> &player)
    {
        if (!player->hasComponent<Comp::PlayerController>() || 
            !player->hasComponent<Comp::Transform>()
        ) return;
        
        auto& controller = player->getComponent<Comp::PlayerController>();
        auto& transform = player->getComponent<Comp::Transform>();
        
        transform.velocity = controller.frameVelocity;
    }
    
}