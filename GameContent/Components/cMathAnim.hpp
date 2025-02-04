//
//
//
//

#pragma once

#include <utility>

#include "Component.hpp"
#include "Entity.hpp"
#include "Debug.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/trigonometric.hpp"

namespace Comp
{
    class MathAnim : public Component 
    {
        public:

            explicit MathAnim(Inst inst) 
            :   Component(std::move(inst))
 
            {}
            
            float easeInOutSine(float x) 
            {
            return -(glm::cos(glm::pi<float>() * x) - 1) / 2;
            }
            
            
    };
}