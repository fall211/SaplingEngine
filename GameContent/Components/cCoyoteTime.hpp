//
//
//
//


#pragma once

#include "Component.hpp"
#include "Entity.hpp"
#include "Debug.hpp"
#include "cMoves.hpp"

namespace Comp
{
    class CoyoteTime : public Component 
    {
        public:
            
            float coyoteTime = 0.25f;
            float coyoteTimer = 0.0f;
            bool expired = false;

            explicit CoyoteTime(const std::shared_ptr<Entity>& inst, float time = 0.25f) 
            :   Component(inst),
                coyoteTime(time)
            {}
            
            void OnAddToEntity() override
            {
                inst->ListenForEvent("UpdateCoyoteTime", OnUpdateCoyoteTime);
                inst->ListenForEvent("ResetCoyoteTime", OnResetCoyoteTime);

            }
            
            
            void OnRemoveFromEntity() override
            {
                inst->RemoveEventCallback("UpdateCoyoteTime", OnUpdateCoyoteTime);
                inst->RemoveEventCallback("ResetCoyoteTime", OnResetCoyoteTime);
            }
            
            
            static void OnUpdateCoyoteTime(Inst inst)
            {
                if (inst->hasComponent<CoyoteTime>() && inst->hasComponent<Moves>())
                {
                    auto& coyote = inst->getComponent<CoyoteTime>();
                    Debug::log("Coyote time: " + std::to_string(coyote.coyoteTimer));
                    if (coyote.coyoteTimer > coyote.coyoteTime)
                    {
                        inst->getComponent<Moves>().grounded = false;
                        coyote.expired = true;
                    }
                }
            }
            
            static void OnResetCoyoteTime(Inst inst)
            {
                if (inst->hasComponent<CoyoteTime>())
                {
                    auto& coyote = inst->getComponent<CoyoteTime>();
                    coyote.coyoteTimer = 0;
                    coyote.expired = false;
                }
            }
    };
}