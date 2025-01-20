//
//
//
//


#pragma once

#include "Component.hpp"
#include "Entity.hpp"
#include "Input/Input.hpp"
#include "Debug.hpp"
#include <cfloat>
#include <utility>

namespace Comp
{
    class PlayerController : public Component 
    {
        public:
        
        
            // stats
            float maxSpeed = 400.0f;
            float acceleration = 3000.0f;
            float groundDeceleration = 1500.0f;
            float airDeceleration = 750.0f;
            float groundingForce = 15.0f;
            float jumpPower = 500.0f;
            float maxFallSpeed = 400.0f;
            float fallAcceleration = 1000.0f;
            float jumpEndEarlyGravityModifier = 3.0f;
            float coyoteTime = 0.15f;
            float jumpBufferTime = 0.15f;
            float dashSpeed = 1200.0f;
            float dashTime = 1.0f;
            
            // frame
            struct FrameInput
            {
                bool jumpDown;
                bool jumpHeld;
                glm::vec2 move;
                bool dashDown;
                glm::vec2 dashDirection;
            } frameInput;
            
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
                return _time > timeDashWasPressed + dashTime;
            }
            
            void flipSpriteX()
            {
                auto& transform = inst->getComponent<Comp::Transform>();
                float x = fabs(transform.scale.x);
                x *= (frameInput.move.x < 0 ? -1 : 1);

                transform.scale = glm::vec3(x, transform.scale.y, transform.scale.z);
            }

            explicit PlayerController(Inst inst) 
            :   Component(std::move(inst))
            {}
            
            void OnAddToEntity() override
            {
                // add tags
                inst->requestAddTag("playercontrolled");
                inst->requestAddTag("player");
                inst->ListenForEvent("Jump", Jump);
                inst->ListenForEvent("Dash", Dash);
            }
            
            void OnRemoveFromEntity() override
            {
                // remove tags
                inst->requestRemoveTag("playercontrolled");
                inst->requestRemoveTag("player");
                inst->RemoveEventCallback("Jump", Jump);
                inst->RemoveEventCallback("Dash", Dash);
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
                }
            }
            
            static void Dash(const Inst& inst)
            {
                if (inst->hasComponent<PlayerController>())
                {
                    auto& controller = inst->getComponent<PlayerController>();
                    
                    controller.airDashUsed = !controller.grounded;
                    controller.timeDashWasPressed = controller._time;
                    controller.frameVelocity = controller.frameInput.dashDirection * controller.dashSpeed;
                }
            }
    
    };
}