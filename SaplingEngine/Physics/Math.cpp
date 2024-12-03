//
//  Math.cpp
//  ECS_Engine
//
//  Created by Tuukka Virtanen on 1/16/24.
//

#include "Math.hpp"
#include <cmath>

auto Math::dist2(const Vector2 vec0, const Vector2 vec1) -> float{
    return (vec0.x - vec1.x) * (vec0.x - vec1.x) + (vec0.y - vec1.x) * (vec0.y - vec1.y);
}

auto Math::dist(const Vector2 vec0, const Vector2 vec1) -> float{
    return sqrtf(dist2(vec0, vec1));
}

auto Vector2::dot(const Vector2& other) const -> float {
    return x * other.x + y * other.y;
}

auto Vector2::magnitude() const -> float {
    return sqrtf(x * x + y * y);
}

auto Vector2::normalized() const -> Vector2 {
    const float len = magnitude();
    return Vector2(x / len, y / len);
}

void Vector2::normalize(){
    const float len = magnitude();
    x /= len;
    y /= len;
}

void Vector2::clamp01(){
    if (magnitude() < 1.0f) return;
    else normalize();
}

void Vector2::clamp(const float lower, const float upper){
    const float len = magnitude();
    if (len > lower && len < upper) return;
    this->normalize();
    if (len > upper) (*this) *= upper;
    else (*this) *= lower;
}

