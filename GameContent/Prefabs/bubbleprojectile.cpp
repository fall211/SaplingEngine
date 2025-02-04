//
//
//
//

#include "bubbleprojectile.hpp"
#include "cLifetime.hpp"



namespace Prefab
{

    BubbleProjectile::BubbleProjectile(const Inst& inst, glm::vec2 position, glm::vec2 direction)
    {
        auto& transform = inst->addComponent<Comp::Transform>(position);
        transform.pivot = Sprout::Pivot::CENTER;
        
        auto& sprite = inst->addComponent<Comp::Sprite>(AssetManager::getTexture("bubble_projectile"));
        sprite.setLayer(Comp::Sprite::Layer::Foreground);
        auto& collider = inst->addComponent<Comp::BBox>(12, 12);
        collider.isStatic = false;
        collider.isTrigger = true;
        collider.collisionEventsEnabled = true;
        
        inst->addComponent<Comp::MathAnim>();
        inst->addComponent<Comp::Gravity>(-1);
        auto& bubble = inst->addComponent<Comp::Bubble>();
        
        float dir = direction.x < 0 ? 1 : -1;
        transform.velocity = glm::vec2(dir * bubble.initialVel, 0);
        
        inst->addComponent<Comp::Lifetime>(bubble.lifetime);
        
        inst->requestAddTag("dynamic");
        inst->requestAddTag("projectile");
    }
    

}

