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
#include <functional>
#include <utility>

namespace Comp
{
    class PlayerController : public Component 
    {
        
        public:
        
        
            // stats
            float maxSpeed = 300.0f;
            float acceleration = 3000.0f;
            float groundDeceleration = 1500.0f;
            float airDeceleration = 750.0f;
            float groundingForce = 15.0f;
            float jumpPower = 500.0f;
            float maxFallSpeed = 800.0f;
            float fallAcceleration = 2000.0f;
            float jumpEndEarlyGravityModifier = 3.0f;
            float coyoteTime = 0.15f;
            float jumpBufferTime = 0.15f;
            float dashSpeed = 800.0f;
            float dashTime = 0.15f;
            float dashCooldown = 1.0f;
            
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
                return _time > timeDashWasPressed + dashCooldown;
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
                inst->ListenForEvent<Inst>("Jump", E_Jump);
                inst->ListenForEvent<Inst>("Dash", E_Dash);
                inst->ListenForEvent<Inst, Inst>("TriggerStay", E_TriggerStay);
                inst->ListenForEvent<Inst, Inst>("TriggerEnter", E_TriggerEnter);
                inst->ListenForEvent<Inst, Inst>("TriggerExit", E_TriggerExit);
            }
            
            void OnRemoveFromEntity() override
            {
                // remove tags
                inst->requestRemoveTag("playercontrolled");
                inst->requestRemoveTag("player");
                inst->requestRemoveTag("dynamic");
                inst->RemoveEventCallback("Jump", E_Jump);
                inst->RemoveEventCallback("Dash", E_Dash);
                inst->RemoveEventCallback("TriggerStay", E_TriggerStay);
                inst->RemoveEventCallback("TriggerEnter", E_TriggerEnter);
                inst->RemoveEventCallback("TriggerExit", E_TriggerExit);
                
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
                    
                    controller.timeDashWasPressed = controller._time;
                    controller.frameVelocity = controller.frameInput.dashDirection * controller.dashSpeed;
                    controller.endedJumpEarly = true;
                }
            }
    
            static void TriggerStay(const Inst& inst, const Inst& other)
            {
                
                if (inst->hasComponent<TransformHierarchy>() && other->hasComponent<TransformHierarchy>())
                {
                    if (Input::isActionDown("pickup"))
                    {
                        auto& instTransform = inst->getComponent<TransformHierarchy>();
                        auto& otherTransform = other->getComponent<TransformHierarchy>();
                        
                        if (otherTransform.parent == nullptr)
                        {
                            otherTransform.setParent(inst);
                            other->PushEvent("Pickup", other);
                        }
                        else 
                        {
                            otherTransform.removeParent();
                            Debug::log("Dropped object");
                            other->PushEvent("Drop", other);
                        }
                        
                    }

                }
            }
            
            static void TriggerEnter(const Inst& inst, const Inst& other)
            {
            }
            
            static void TriggerExit(const Inst& inst, const Inst& other)
            {
            }
            
        private:
        
            //events
            std::function<void(Inst)> E_Jump = PlayerController::Jump;
            std::function<void(Inst)> E_Dash = PlayerController::Dash;
            std::function<void(Inst, Inst)> E_TriggerStay = PlayerController::TriggerStay;
            std::function<void(Inst, Inst)> E_TriggerEnter = PlayerController::TriggerEnter;
            std::function<void(Inst, Inst)> E_TriggerExit = PlayerController::TriggerExit;
    };
}