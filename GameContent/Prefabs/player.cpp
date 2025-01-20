//
//
//
//


#include "player.hpp"


namespace Prefab
{

    Player::Player(Inst inst)
    {
        init(inst);
    }
    
    void Player::init(Inst inst)
    {
        auto& transform = inst->addComponent<Comp::Transform>(glm::vec2(0, 0), glm::vec2(0, 0));
        transform.pivot = Sprout::Pivot::TOP_CENTER;
        inst->addComponent<Comp::Sprite>(AssetManager::getTexture("player_run"), 8);
        inst->addComponent<Comp::BBox>(24, 24).isStatic = false;
        

        // movement
        inst->addComponent<Comp::PlayerController>();
        
    }
}

