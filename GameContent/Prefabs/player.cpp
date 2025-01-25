//
//
//
//


#include "player.hpp"

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
        transform.scale = glm::vec3(3, 3, 1);
        inst->addComponent<Comp::Sprite>(AssetManager::getTexture("player_run"), 8);
        inst->addComponent<Comp::BBox>(24, 24).isStatic = false;
        

        // movement
        inst->addComponent<Comp::PlayerController>();
        
    }
}

