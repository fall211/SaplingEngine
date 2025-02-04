//
//
//
//

#pragma once

#include "Entity.hpp"
#include "EntityManager.hpp"
#include "Seedbank/AssetManager.hpp"

#include "Component.hpp"
#include "cMathAnim.hpp"
#include "cGravity.hpp"
#include "cBubble.hpp"
#include "cLifetime.hpp"
#include <cstddef>
#include <memory>

namespace Prefab
{
    class BubbleProjectile
    {
        public:
            BubbleProjectile(const Inst& inst, glm::vec2 position, glm::vec2 direction);                
        
    };
    
}

