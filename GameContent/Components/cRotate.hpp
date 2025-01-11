//
//
//
//

#pragma once

#include "Component.hpp"

namespace Comp
{

    class Rotate : public Component 
    {
        public:
            float rotationSpeed = 0.0f;
            explicit Rotate(float speed) : rotationSpeed(speed) {}
    };

}