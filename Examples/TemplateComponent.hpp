//
//
//
//

#pragma once

#include "ECS/Component.hpp"
#include "ECS/Entity.hpp"
#include "Utility/Debug.hpp"

namespace Comp
{
    
    struct COMPNAME : public Component 
    {
        explicit COMPNAME(Inst inst) 
        :   Component(std::move(inst))
        {}
        
        /*
        void OnAddToEntity() override
        {
            inst->requestAddTag(TAG);
            inst->ListenForEvent<Inst, float>("EVENT", E_EVENT);
        }
        
        
        void OnRemoveFromEntity() override
        {
            inst->requestRemoveTag(TAG);
            inst->RemoveEventCallback("EVENT", E_EVENT);
        }
        
        static void EVENT(const Inst& inst, const float value)
        {

        }

        
    private:
        std::function<void(Inst, float)> E_EVENT = COMPNAME::EVENT;
        
        */
    };
    
    
}