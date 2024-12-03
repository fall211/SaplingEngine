//
//  Physics.cpp
//  SaplingEngine, Twig Physics
//

#include "Physics.hpp"


auto Physics2D::bBoxCollision(const std::shared_ptr<Entity>& e0, const std::shared_ptr<Entity>& e1) -> Vector2 {
    /**
    Detects the overlap of the bounding boxes of the two entities e0 and e1.
    Return: The overlap vector.
    **/
    if (e0->getId() == e1->getId()) return Vector2::zero(); // ignore self collision

    const Vector2 dr = e0->getComponent<CTransform>().position - e1->getComponent<CTransform>().position;
    const float dx = fabs(dr.x);
    const float dy = fabs(dr.y);

    const float xOverlap = e0->getComponent<CBBox>().w / 2.0f + e1->getComponent<CBBox>().w / 2.0f - dx;
    const float yOverlap = e0->getComponent<CBBox>().h / 2.0f + e1->getComponent<CBBox>().h / 2.0f - dy;

    if (xOverlap > 0 && yOverlap > 0) {
        return Vector2(xOverlap, yOverlap);
    }
    return Vector2::zero();
}

auto bCircleCollision(const std::shared_ptr<Entity>& e0, const std::shared_ptr<Entity>& e1) -> Vector2 {
    /**
    Detects the overlap of the bounding circles of the two entities e0 and e1.
    Return: The overlap vector.
    **/
    
    if (e0->getId() == e1->getId()) {
        return Vector2::zero();
    }
    
    const Vector2 pos0 = e0->getComponent<CTransform>().position;
    const Vector2 pos1 = e1->getComponent<CTransform>().position;

    const float rsum = e0->getComponent<CBCircle>().radius + e1->getComponent<CBCircle>().radius;
    const float dist2 = Math::dist2(pos0, pos1);
    
    if (dist2 <= rsum * rsum) {
        const float dist = std::sqrt(dist2);
        Vector2 overlap = (pos0 - pos1) * ((rsum - dist) / dist);
        return overlap;
    }
    return Vector2::zero();
}

auto bBoxCircleCollision(const std::shared_ptr<Entity>& eBox, const std::shared_ptr<Entity>& eCircle) -> Vector2 {
    /**
    Detects the overlap of the bounding box of eBox and the bounding circle of eCircle.
    Return: The overlap vector.
    **/
    
    const Vector2 circlePos = eCircle->getComponent<CTransform>().position;
    const float circleR = eCircle->getComponent<CBCircle>().radius;

    const Vector2 boxPos = eBox->getComponent<CTransform>().position;
    const float boxW = eBox->getComponent<CBBox>().w;
    const float boxH = eBox->getComponent<CBBox>().h;
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
    
    if (distance <= circleR) {
        float overlap = circleR - distance;
        Vector2 overlapVector(distX, distY);
        if (distance != 0) {
            overlapVector.normalize();
            overlapVector *= overlap;
        } else {
            overlapVector = Vector2(circleR, 0);
        }
        return overlapVector;
    }
    
    return Vector2::zero();
}