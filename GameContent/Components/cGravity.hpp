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
    class Gravity : public Component 
    {
        public:
            float gravity;
            explicit Gravity(const std::shared_ptr<Entity>& inst, float grav = 9.8f * 60) 
            :   Component(inst), 
                gravity(grav) 
            {}
            
            void OnAddToEntity() override
            {
                inst->requestAddTag("gravityAffected");
            }
            
            
            void OnRemoveFromEntity() override
            {
                inst->requestRemoveTag("gravityAffected");
            }
    };
}