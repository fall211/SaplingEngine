//
//
//
//

#include "bubbleprojectile.hpp"
#include "Debug.hpp"
#include "cLifetime.hpp"



namespace Prefab
{

    BubbleProjectile::BubbleProjectile(const Inst& inst, glm::vec2 position, glm::vec2 direction)
    {
        // add noise to the position
        position.x += (rand() % 10) - 5;
        position.y += (rand() % 10) - 5;
        
        auto& transform = inst->addComponent<Comp::Transform>(position);
        transform.pivot = Sprout::Pivot::CENTER;
        
        auto& sprite = inst->addComponent<Comp::Sprite>(AssetManager::getTexture("bubble_projectile"));
        sprite.setLayer(Comp::Sprite::Layer::Foreground);
        auto& collider = inst->addComponent<Comp::BBox>(6, 6);
        collider.isStatic = false;
        collider.isTrigger = true;
        collider.collisionEventsEnabled = true;
        
        inst->addComponent<Comp::MathAnim>();
        inst->addComponent<Comp::Gravity>(-1);
        auto& bubble = inst->addComponent<Comp::Bubble>();
        
        float dir = direction.x < 0 ? 1 : -1;
        float dirY = (rand() % 25) - 12.5f;
        transform.velocity = glm::vec2(dir * bubble.initialVel, dirY);
        
        inst->addComponent<Comp::Lifetime>(bubble.lifetime - (rand() % 10) / 10.0f);
        
        inst->requestAddTag("dynamic");
        inst->requestAddTag("projectile");
        // Debug::log("BubbleProjectile created");
    }
    

}

