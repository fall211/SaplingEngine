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
    
    class Lifetime : public Component 
    {
        public:
            float lifetime;
            explicit Lifetime(Inst inst, float lifetime) 
            :   Component(std::move(inst)),
                lifetime(lifetime)
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