//
//
//
//

#pragma once

#include <utility>

#include "Component.hpp"
#include "Entity.hpp"
#include "Debug.hpp"

namespace Comp
{
    class Gravity : public Component 
    {
        public:
            float gravity;
            explicit Gravity(Inst inst, float grav = 9.8f) 
            :   Component(std::move(inst)), 
                gravity(grav) 
            {}
            
            void OnAddToEntity() override
            {
                inst->requestAddTag("gravityaffected");
            }
            
            
            void OnRemoveFromEntity() override
            {
                inst->requestRemoveTag("gravityaffected");
            }
    };
}