//
//
//
//


#pragma once

#include "Component.hpp"
#include "cPickup.hpp"
#include "Entity.hpp"
#include "Input/Input.hpp"
#include "Debug.hpp"
#include <cfloat>
#include <functional>
#include <utility>
#include "Audio/AudioEngine.hpp"

namespace Comp
{
    class PlayerController : public Component 
    {
        
        public:
        
        
            // stats
            float maxSpeed = 200.0f;
            float acceleration = 2000.0f;
            float groundDeceleration = 1000.0f;
            float airDeceleration = 500.0f;
            float groundingForce = 5.0f;
            float jumpPower = 400.0f;
            float maxFallSpeed = 533.34f;
            float fallAcceleration = 1333.34f;
            float jumpEndEarlyGravityModifier = 3.0f;
            float coyoteTime = 0.15f;
            float jumpBufferTime = 0.15f;
            float dashSpeed = 533.34f;
            float dashTime = 0.15f;
            float dashCooldown = 0.3f;
            
            // frame
            struct FrameInput
            {
                bool jumpDown;
                bool jumpHeld;
                glm::vec2 move;
                bool dashDown;
                glm::vec2 dashDirection;
            } frameInput;
            
            struct CollisionState
            {
                bool groundHit;
                bool ceilingHit;
            } collisionState;
            
            glm::vec2 frameVelocity;
            float _time;
            
            // collisions
            float frameLeftGround = FLT_MIN;
            bool grounded;
            
            // jumping
            bool jumpToConsume;
            bool bufferedJumpUsable;
            bool endedJumpEarly;
            bool coyoteUsable;
            float timeJumpWasPressed;
            
            // dash
            float timeDashWasPressed;
            bool airDashUsed;
            bool hasDash;
            
            bool HasBufferedJump() const
            {
                return bufferedJumpUsable && _time < (timeJumpWasPressed + jumpBufferTime);
            }
            
            bool CanUseCoyote() const 
            {
                return coyoteUsable && !grounded && _time < frameLeftGround + coyoteTime;
            }
            
            bool CanUseDash() const
            {
                return hasDash && _time > timeDashWasPressed + dashCooldown;
            }
            
            bool IsDashing() const
            {
                return _time < timeDashWasPressed + dashTime;
            }
            
            void flipSpriteX()
            {
                auto& transform = inst->getComponent<Comp::Transform>();
                float x = fabs(transform.scale.x);
                x *= (frameInput.move.x < 0 ? -1 : 1);

                transform.scale = glm::vec3(x, transform.scale.y, transform.scale.z);
                if (inst->hasComponent<TransformHierarchy>())
                {
                    if (!inst->getComponent<TransformHierarchy>().children.empty())
                    {
                        for (auto& child : inst->getComponent<TransformHierarchy>().children)
                        {
                            auto& childTransform = child->getComponent<Comp::Transform>();
                            float x = fabs(childTransform.scale.x);
                            x *= (frameInput.move.x < 0 ? -1 : 1);
                            childTransform.scale = glm::vec3(x, childTransform.scale.y, childTransform.scale.z);
                        }
                    }
                }
            }

            explicit PlayerController(Inst inst) 
            :   Component(std::move(inst))
            {}
            
            void OnAddToEntity() override
            {
                // add tags
                inst->requestAddTag("playercontrolled");
                inst->requestAddTag("player");
                inst->requestAddTag("dynamic");
                inst->requestAddTag("physicsenabled");
                inst->ListenForEvent<Inst>("Jump", E_Jump);
                inst->ListenForEvent<Inst>("Dash", E_Dash);
                inst->ListenForEvent<Inst>("FloorCollision", E_FloorCollision);
                inst->ListenForEvent<Inst>("CeilingCollision", E_CeilingCollision);
            }
            
            void OnRemoveFromEntity() override
            {
                // remove tags
                inst->requestRemoveTag("playercontrolled");
                inst->requestRemoveTag("player");
                inst->requestRemoveTag("dynamic");
                inst->requestRemoveTag("physicsenabled");
                inst->RemoveEventCallback("Jump", E_Jump);
                inst->RemoveEventCallback("Dash", E_Dash);
                inst->RemoveEventCallback("FloorCollision", E_FloorCollision);
                inst->RemoveEventCallback("CeilingCollision", E_CeilingCollision);
            }
            
            static void Jump(const Inst& inst)
            {
                if (inst->hasComponent<PlayerController>())
                {
                    auto& controller = inst->getComponent<PlayerController>();
                    
                    controller.endedJumpEarly = false;
                    controller.timeJumpWasPressed = 0;
                    controller.bufferedJumpUsable = false;
                    controller.coyoteUsable = false;
                    controller.frameVelocity.y = -controller.jumpPower;
                    AudioEngine::getInstance()->playSound("jump");
                }
            }
            
            static void Dash(const Inst& inst)
            {
                if (inst->hasComponent<PlayerController>())
                {
                    auto& controller = inst->getComponent<PlayerController>();
                    
                    controller.timeDashWasPressed = controller._time;
                    controller.frameVelocity = controller.frameInput.dashDirection * controller.dashSpeed;
                    controller.endedJumpEarly = true;
                    controller.hasDash = false;
                    AudioEngine::getInstance()->playSound("dash", 2);
                }
            }
            
            static void FloorCollision(const Inst& inst)
            {
                if (inst->hasComponent<PlayerController>())
                {
                    auto& controller = inst->getComponent<PlayerController>();
                    
                    controller.collisionState.groundHit = true;
                }
            }
            
            static void CeilingCollision(const Inst& inst)
            {
                if (inst->hasComponent<PlayerController>())
                {
                    auto& controller = inst->getComponent<PlayerController>();
                    
                    controller.collisionState.ceilingHit = true;
                }
            }

            
        private:
        
            //events
            std::function<void(Inst)> E_Jump = PlayerController::Jump;
            std::function<void(Inst)> E_Dash = PlayerController::Dash;
            std::function<void(Inst)> E_FloorCollision = PlayerController::FloorCollision;
            std::function<void(Inst)> E_CeilingCollision = PlayerController::CeilingCollision;
    };
}