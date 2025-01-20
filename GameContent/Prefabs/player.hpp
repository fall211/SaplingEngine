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
            void init(Inst inst);
            
        public:
            Player(Inst inst);
            
            static void speedUpRotation(Inst inst);
        
    };
    
}

