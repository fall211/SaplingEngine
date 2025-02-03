//
//
//
//


#include "player.hpp"
#include "Component.hpp"
#include "cPickupHandler.hpp"

#include <utility>


namespace Prefab
{

    Player::Player(const Inst& inst)
    {
        init(inst);
    }
    
    void Player::init(const Inst& inst)
    {
        auto& transform = inst->addComponent<Comp::Transform>(glm::vec2(0, 0), glm::vec2(0, 0));
        transform.pivot = Sprout::Pivot::CENTER;
        // transform.scale = glm::vec3(3, 3, 1);
        auto& sprite = inst->addComponent<Comp::Sprite>(AssetManager::getTexture("player_run"), 8);
        sprite.setLayer(Comp::Sprite::Layer::Player);
        auto& collider = inst->addComponent<Comp::BBox>(24, 24);
        collider.isStatic = false;
        collider.interactWithTriggers = true;
        
        inst->addComponent<Comp::TransformHierarchy>();
        inst->addComponent<Comp::PickupHandler>();

        // movement
        inst->addComponent<Comp::PlayerController>();
        
    }
}

