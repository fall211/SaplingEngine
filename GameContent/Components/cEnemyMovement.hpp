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
    class EnemyMovement : public Component 
    {
        public:
            float speed = 55.0f;
            float detectRange = 100.0f;
            int moveDirection = 1;
            explicit EnemyMovement(Inst inst) 
            :   Component(std::move(inst))
            {}
    };
}