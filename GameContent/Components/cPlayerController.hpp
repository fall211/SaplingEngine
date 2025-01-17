//
//
//
//


#pragma once

#include "Component.hpp"
#include "Entity.hpp"
#include "Debug.hpp"
#include "cCoyoteTime.hpp"
#include "cMoves.hpp"

namespace Comp
{
    class PlayerController : public Component 
    {
        public:

            explicit PlayerController(const std::shared_ptr<Entity>& inst) 
            :   Component(inst)
            {}
            
            void OnAddToEntity() override
            {
                // add tags
                inst->requestAddTag("playercontrolled");
                
                // add events
                inst->ListenForEvent("jump", OnJump);
            }
            
            
            void OnRemoveFromEntity() override
            {
                // remove tags
                inst->requestRemoveTag("playercontrolled");
                
                // remove events
                inst->RemoveEventCallback("jump", OnJump);
            }
            
            
            static void OnJump(Inst inst)
            {
                if (inst -> hasComponent<PlayerController>() && 
                    inst -> hasComponent<Transform>() &&
                    inst -> hasComponent<Moves>()
                )
                {
                    auto& player = inst->getComponent<PlayerController>();
                    auto& transform = inst->getComponent<Transform>();
                    auto& moves = inst->getComponent<Moves>();
                    
                    if (moves.grounded)
                    {
                        moves.grounded = false;
                        transform.velocity.y = -moves.jumpStr;
                        if (inst->hasComponent<CoyoteTime>())
                        {
                            inst->getComponent<CoyoteTime>().coyoteTimer = 0;
                        }
                    }
                }
            }
            
            static void OnMove(Inst inst)
            {
                if (inst->hasComponent<PlayerController>() && inst -> hasComponent<Transform>())
                {
                    auto& player = inst->getComponent<PlayerController>();
                    auto& transform = inst->getComponent<Transform>();
                    
                    
                    
                }
            }
            
    };
}