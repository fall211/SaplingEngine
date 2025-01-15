//
//
//
//


#include "Component.hpp"
#include "cRotate.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "Seedbank/AssetManager.hpp"

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

