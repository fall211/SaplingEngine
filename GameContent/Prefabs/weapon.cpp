//
//
//
//


#include "weapon.hpp"
#include "Component.hpp"
#include "Debug.hpp"
#include "cGravity.hpp"
#include "cPickup.hpp"
#include "cWeapon.hpp"


namespace Prefab
{

    Weapon::Weapon(const Inst& inst)
    {
        init(inst);
    }
    
    void Weapon::init(const Inst& inst)
    {
        auto& transform = inst->addComponent<Comp::Transform>(glm::vec2(0, 0), glm::vec2(0, 0));
        transform.pivot = Sprout::Pivot::CENTER;
        auto& sprite = inst->addComponent<Comp::Sprite>(AssetManager::getTexture("weapon_basic"));
        sprite.setLayer(Comp::Sprite::Layer::Midground);
        auto& collider = inst->addComponent<Comp::BBox>(24, 24);
        collider.isStatic = false;
        collider.isTrigger = true;
        
        inst->addComponent<Comp::TransformHierarchy>();
        inst->addComponent<Comp::Gravity>();
        inst->addComponent<Comp::Pickup>();
        inst->requestAddTag("dynamic");
        inst->addComponent<Comp::Weapon>();
    }
}

