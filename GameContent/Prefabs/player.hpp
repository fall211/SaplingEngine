//
//
//
//


#include "Entity.hpp"
#include "EntityManager.hpp"
#include "Seedbank/AssetManager.hpp"

#include "Component.hpp"
#include "GameComponents.hpp"
#include "cPlayerController.hpp"

#include <cstddef>
#include <memory>

namespace Prefab
{
    class Player
    {
        private:            
            void init(const Inst& inst);
            
        public:
            Player(const Inst& inst);
            
        
    };
    
}

