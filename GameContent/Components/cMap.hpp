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
    class Map : public Component 
    {
        public:
            size_t width;
            size_t height;
            const char* tileset;
            
            explicit Map(Inst inst) 
            :   Component(std::move(inst))
            {}
            
            void OnAddToEntity() override
            {
                inst->requestAddTag("map");
            }
            
            
            void OnRemoveFromEntity() override
            {
                inst->requestRemoveTag("map");
            }
    };
}