//
//
//
//


#include "player.hpp"
#include "Seedbank/AssetManager.hpp"
#include "GameComponents.hpp"
#include "cLowestY.hpp"

namespace Prefab
{

    Player::Player(Inst inst)
    {
        init(inst);
    }
    
    void Player::init(Inst inst)
    {
        inst->addComponent<Comp::Transform>(glm::vec2(0, 0), glm::vec2(0, 0));
        inst->addComponent<Comp::Sprite>(AssetManager::getTexture("test"));
        inst->addComponent<Comp::BBox>(64, 64);
        inst->addComponent<Comp::Gravity>();
        inst->addComponent<Comp::LowestY>();
        
    }
        
    void Player::speedUpRotation(Inst inst)
    {
        inst->getComponent<Comp::Rotate>().rotationSpeed += 0.16f;
    }
}

