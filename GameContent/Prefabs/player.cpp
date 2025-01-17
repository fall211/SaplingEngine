//
//
//
//


#include "player.hpp"
#include "Seedbank/AssetManager.hpp"
#include "GameComponents.hpp"
#include "cLowestY.hpp"
#include "cPlayerController.hpp"

namespace Prefab
{

    Player::Player(Inst inst)
    {
        init(inst);
    }
    
    void Player::init(Inst inst)
    {
        inst->addComponent<Comp::Transform>(glm::vec2(0, 0), glm::vec2(0, 0));
        inst->addComponent<Comp::Sprite>(AssetManager::getTexture("player"));
        auto& bbox = inst->addComponent<Comp::BBox>(64, 64);
        bbox.isStatic = false;
        inst->addComponent<Comp::Gravity>();
        inst->addComponent<Comp::LowestY>();
        inst->addComponent<Comp::PlayerController>();
        inst->addComponent<Comp::Moves>();
        inst->addComponent<Comp::CoyoteTime>();
        
    }
        
    void Player::speedUpRotation(Inst inst)
    {
        inst->getComponent<Comp::Rotate>().rotationSpeed += 0.16f;
    }
}

