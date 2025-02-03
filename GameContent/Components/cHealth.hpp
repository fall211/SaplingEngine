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
    class Health : public Component 
    {
        public:
            float health;
            explicit Health(Inst inst, float hp) 
            :   Component(std::move(inst)), 
                health(hp) 
            {}
    };
}