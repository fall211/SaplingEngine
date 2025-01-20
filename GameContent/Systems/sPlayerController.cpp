//
//
//
//



#include "sPlayerController.hpp"
#include "Debug.hpp"


namespace System::PlayerController
{    
    void Update(const std::shared_ptr<Entity>& player, EntityList& entities, const std::shared_ptr<Input>& input, float dt)
    {
        if (!player->hasComponent<Comp::PlayerController>() || 
            !player->hasComponent<Comp::Transform>() ||
            !player->hasComponent<Comp::BBox>()
        ) return;
        
        auto& controller = player->getComponent<Comp::PlayerController>();
        
        controller._time += dt;
        
        GatherInput(player, input);
        CheckCollision(player, entities);
        HandleJump(player);
        HandleDirection(player, dt);
        HandleDash(player);
        HandleGravity(player, dt);
        ApplyMovement(player);
        
    }
    
    void GatherInput(const std::shared_ptr<Entity>& player, const std::shared_ptr<Input>& input)
    {
        if (!player->hasComponent<Comp::PlayerController>()) return;
        
        auto& controller = player->getComponent<Comp::PlayerController>();
        
        controller.frameInput.jumpDown = input->isActionDown("jump");
        controller.frameInput.jumpHeld = input->isAction("jump");
        controller.frameInput.move = glm::vec2(input->getAxis("horizontal"), input->getAxis("vertical"));
        
        // dash
        controller.frameInput.dashDown = input->isActionDown("dash");
        controller.frameInput.dashDirection = glm::normalize(glm::vec2(input->getAxis("horizontal"), input->getAxis("vertical")));
        
        
        if (controller.frameInput.jumpDown)
        {
            controller.jumpToConsume = true;
            controller.timeJumpWasPressed = controller._time;
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
                // figure out if the collision is on the ground or ceiling
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
            controller.airDashUsed = false;
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
            player->PushEvent("Jump");
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
        }
        else
        {
            controller.frameVelocity.x = moveTowards(controller.frameVelocity.x, controller.frameInput.move.x * controller.maxSpeed, controller.acceleration * dt);
            controller.flipSpriteX();
        }
    }
    
    void HandleDash(const std::shared_ptr<Entity> &player)
    {
        if (!player->hasComponent<Comp::PlayerController>()
        ) return;
        
        auto& controller = player->getComponent<Comp::PlayerController>();
        
        if (controller.frameInput.dashDown && controller.frameInput.dashDirection != glm::vec2(0, 0) && controller.CanUseDash())
        {
            player->PushEvent("Dash");
        }
    }
    
    void HandleGravity(const std::shared_ptr<Entity> &player, float dt)
    {
        if (!player->hasComponent<Comp::PlayerController>()
        ) return;
        
        auto& controller = player->getComponent<Comp::PlayerController>();

        if (controller.grounded && controller.frameVelocity.y >= 0.0f)
        {
            controller.frameVelocity.y = controller.groundingForce;
        }
        else
        {
            auto inAirGravity = controller.fallAcceleration;
            if ((controller.endedJumpEarly || controller.airDashUsed) && controller.frameVelocity.y <= 0)
            {
                inAirGravity *= controller.jumpEndEarlyGravityModifier;
            }
            
            auto moveTowards = [](float current, float target, float maxDelta) -> float {
                if (std::abs(target - current) <= maxDelta) {
                    return target;
                }
                return current + glm::sign(target - current) * maxDelta;
            };
                        
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