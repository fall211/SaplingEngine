//
//
//
//


#include "enemy.hpp"
#include "Component.hpp"
#include "Debug.hpp"
#include "cBrain.hpp"
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

    }

    // forest spirit

    ForestSpirit::ForestSpirit(const Inst& inst) : Enemy(inst)
    {
        spawn(inst);
    }
    void ForestSpirit::spawn(const Inst& inst)
    {
        auto& sprite = inst->addComponent<Comp::Sprite>(AssetManager::getTexture("spirit_idle"), 6);
        sprite.animationSpeed = 6;
        sprite.setLayer(Comp::Sprite::Layer::Midground);
        auto& collider = inst->addComponent<Comp::BBox>(12, 20);
        collider.isStatic = false;
        collider.isTrigger = true;

        inst->addComponent<Comp::Gravity>();
        inst->addComponent<Comp::Health>(3);
        inst->addComponent<Comp::Brain>();
        inst->addComponent<Comp::Damage>(1).enabled = false;

        inst->requestAddTag("dynamic");
        inst->requestAddTag("enemy");
    }

    // hiisi

    Hiisi::Hiisi(const Inst& inst) : Enemy(inst) 
    {
        spawn(inst);
    }

    void Hiisi::spawn(const Inst& inst)
    {
        auto& sprite = inst->addComponent<Comp::Sprite>(AssetManager::getTexture("hiisi"), 2);
        sprite.animationSpeed = 60;
        sprite.setLayer(Comp::Sprite::Layer::Midground);
        auto& collider = inst->addComponent<Comp::BBox>(24, 24);
        collider.isStatic = false;

        inst->addComponent<Comp::Gravity>();
        inst->addComponent<Comp::Health>(3);
        inst->addComponent<Comp::Brain>();
        inst->addComponent<Comp::MathAnim>();
        inst->requestAddTag("dynamic");
        inst->requestAddTag("enemy");
    }

    // serpent

    Serpent::Serpent(const Inst& inst) : Enemy(inst) 
    {
        spawn(inst);
    }

    void Serpent::spawn(const Inst& inst)
    {
        auto& sprite = inst->addComponent<Comp::Sprite>(AssetManager::getTexture("serpent"), 2);
        sprite.animationSpeed = 60;
        sprite.setLayer(Comp::Sprite::Layer::Midground);
        auto& collider = inst->addComponent<Comp::BBox>(24, 24);
        collider.isStatic = false;

        inst->addComponent<Comp::Gravity>();
        inst->addComponent<Comp::Health>(3);
        inst->addComponent<Comp::Brain>();
        inst->addComponent<Comp::MathAnim>();
        inst->requestAddTag("dynamic");
        inst->requestAddTag("enemy");
    }

    // frost wolf

    FrostWolf::FrostWolf(const Inst& inst) : Enemy(inst) 
    {
        spawn(inst);
    }

    void FrostWolf::spawn(const Inst& inst)
    {
        auto& sprite = inst->addComponent<Comp::Sprite>(AssetManager::getTexture("frostwolf"), 2);
        sprite.animationSpeed = 60;
        sprite.setLayer(Comp::Sprite::Layer::Midground);
        auto& collider = inst->addComponent<Comp::BBox>(24, 24);
        collider.isStatic = false;

        inst->addComponent<Comp::Gravity>();
        inst->addComponent<Comp::Health>(3);
        inst->addComponent<Comp::Brain>();
        inst->addComponent<Comp::MathAnim>();
        inst->requestAddTag("dynamic");
        inst->requestAddTag("enemy");
    }



}
