//
//
//
//

#pragma once

#include "Component.hpp"
#include "Entity.hpp"
#include "Debug.hpp"

namespace Comp
{
    
    class COMPNAME : public Component 
    {
        public:
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