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
    class Damage : public Component 
    {
        public:
            float damage;
            explicit Damage(Inst inst, float dmg) 
            :   Component(std::move(inst)), 
                damage(dmg) 
            {}
    };
}