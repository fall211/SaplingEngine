//
//
//
//



#include "sPlayerController.hpp"
#include "Component.hpp"
#include "Debug.hpp"
#include "glm/geometric.hpp"


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
        
    }
    
    void GatherInput(const std::shared_ptr<Entity>& player)
    {
        if (!player->hasComponent<Comp::PlayerController>()) return;
        
        auto& controller = player->getComponent<Comp::PlayerController>();
        
        controller.frameInput.jumpDown = Input::isActionDown("jump");
        controller.frameInput.jumpHeld = Input::isAction("jump");
        controller.frameInput.move = glm::vec2(Input::getAxis("horizontal"), Input::getAxis("vertical"));
        
        // dash
        controller.frameInput.dashDown = Input::isActionDown("dash") || Input::getMouseDown(Input::MouseButton::LEFT);
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
        
        for (auto& other : entities)
        {
            if (other->getId() == player->getId()) continue;
            
            if (!other->hasComponent<Comp::BBox>() || 
                !other->hasComponent<Comp::Transform>()
            ) continue;
            
            auto& transform_other = other->getComponent<Comp::Transform>();
            
            glm::vec2 overlap = Physics2D::bBoxCollision(player, other);
            
            if (overlap.x != 0 || overlap.y != 0)
            {
                if (other->getComponent<Comp::BBox>().isTrigger) continue;
                if (transform.position.y < transform_other.position.y) groundHit = true;
                else ceilingHit = true;
                break;
            }
        }
        
        if (ceilingHit) 
        {
            Debug::log("ceiling hit");
            controller.frameVelocity.y = fmax(0, controller.frameVelocity.y);
        }
        
        if (!controller.grounded && groundHit)
        {
            controller.grounded = true;
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