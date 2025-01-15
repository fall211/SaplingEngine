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
    class LowestY : public Component 
    {
        public:
            float y_level = 300.0f;
            explicit LowestY(const std::shared_ptr<Entity>& inst) 
            :   Component(inst)
            {}
            
            void OnAddToEntity() override
            {
                inst->requestAddTag("stopsinking");
            }
            
            
            void OnRemoveFromEntity() override
            {
                inst->requestRemoveTag("stopsinking");
            }
    };
}