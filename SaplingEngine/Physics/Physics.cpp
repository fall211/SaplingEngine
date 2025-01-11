//
//  Physics.cpp
//  SaplingEngine, Twig Physics
//

#include "Physics.hpp"
#include "glm/geometric.hpp"


auto Physics2D::bBoxCollision(const std::shared_ptr<Entity>& e0, const std::shared_ptr<Entity>& e1) -> glm::vec2
{
    if (e0->getId() == e1->getId()) return {0, 0}; // ignore self collision

    const glm::vec2 dr = e0->getComponent<Comp::Transform>().position - e1->getComponent<Comp::Transform>().position;
    const float dx = fabs(dr.x);
    const float dy = fabs(dr.y);

    const float xOverlap = e0->getComponent<Comp::BBox>().w / 2.0f + e1->getComponent<Comp::BBox>().w / 2.0f - dx;
    const float yOverlap = e0->getComponent<Comp::BBox>().h / 2.0f + e1->getComponent<Comp::BBox>().h / 2.0f - dy;

    if (xOverlap > 0 && yOverlap > 0) 
    {
        return {xOverlap, yOverlap};
    }
    return {0, 0};
}

auto bCircleCollision(const std::shared_ptr<Entity>& e0, const std::shared_ptr<Entity>& e1) -> glm::vec2
{
    /**
    Detects the overlap of the bounding circles of the two entities e0 and e1.
    Return: The overlap vector.
    **/
    
    if (e0->getId() == e1->getId()) return {0, 0}; // ignore self collision
    
    const glm::vec2 pos0 = e0->getComponent<Comp::Transform>().position;
    const glm::vec2 pos1 = e1->getComponent<Comp::Transform>().position;

    const float rsum = e0->getComponent<Comp::BCircle>().radius + e1->getComponent<Comp::BCircle>().radius;
    const float dist2 = glm::distance(pos0, pos1);
    
    if (dist2 <= rsum * rsum) 
    {
        const float dist = std::sqrt(dist2);
        glm::vec2 overlap = (pos0 - pos1) * ((rsum - dist) / dist);
        return overlap;
    }
    return {0, 0};
}

auto bBoxCircleCollision(const std::shared_ptr<Entity>& eBox, const std::shared_ptr<Entity>& eCircle) -> glm::vec2
{
    
    const glm::vec2 circlePos = eCircle->getComponent<Comp::Transform>().position;
    const float circleR = eCircle->getComponent<Comp::BCircle>().radius;

    const glm::vec2 boxPos = eBox->getComponent<Comp::Transform>().position;
    const float boxW = eBox->getComponent<Comp::BBox>().w;
    const float boxH = eBox->getComponent<Comp::BBox>().h;
    float boxLeft = boxPos.x - boxW / 2.0f;
    float boxRight = boxPos.x + boxW / 2.0f;
    float boxTop = boxPos.y - boxH / 2.0f;
    float boxBottom = boxPos.y + boxH / 2.0f;
    
    float testX = circlePos.x;
    float testY = circlePos.y;
    
    if (circlePos.x < boxLeft) 
        testX = boxLeft;
        
    else if (circlePos.x > boxRight) 
        testX = boxRight;
        
    if (circlePos.y < boxTop) 
        testY = boxTop;
        
    else if (circlePos.y > boxBottom) 
        testY = boxBottom;
    
    float distX = circlePos.x - testX;
    float distY = circlePos.y - testY;
    float distance = sqrt((distX*distX) + (distY*distY));
    
    if (distance <= circleR)
    {
        float overlap = circleR - distance;
        glm::vec2 overlapVector(distX, distY);
        if (distance != 0) 
        {
            overlapVector = glm::normalize(overlapVector);
            overlapVector *= overlap;
        } 
        else 
        {
            overlapVector = glm::vec2(circleR, 0);
        }
        return overlapVector;
    }
    
    return {0, 0};
}