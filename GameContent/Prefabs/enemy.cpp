//
//
//
//


#include "enemy.hpp"
#include "Component.hpp"
#include "cGravity.hpp"
#include "cHealth.hpp"

#include <utility>


namespace Prefab
{

    Enemy::Enemy(const Inst& inst)
    {
        init(inst);
    }
    
    void Enemy::init(const Inst& inst)
    {
        auto& transform = inst->addComponent<Comp::Transform>(glm::vec2(0, 0), glm::vec2(0, 0));
        transform.pivot = Sprout::Pivot::CENTER;
        
        auto& sprite = inst->addComponent<Comp::Sprite>(AssetManager::getTexture("hiisi"), 2);
        sprite.animationSpeed = 60;
        sprite.setLayer(Comp::Sprite::Layer::Midground);
        auto& collider = inst->addComponent<Comp::BBox>(24, 24);
        collider.isStatic = false;
        
        inst->addComponent<Comp::Gravity>();
        inst->addComponent<Comp::Health>(10);
        inst->addComponent<Comp::EnemyMovement>();
        inst->addComponent<Comp::MathAnim>();
        inst->requestAddTag("dynamic");
        inst->requestAddTag("enemy");
    }
    
    void Enemy::spawn(const Inst& inst)
    {
        auto& transform = inst->getComponent<Comp::Transform>();
        transform.position = glm::vec2(100, 100);
    }
}

