//
//
//
//

#pragma once

#include "Component.hpp"

class CRotate : public Component {
public:
    float rotationSpeed = 0.0f;
    explicit CRotate(float speed) : rotationSpeed(speed) {}
};