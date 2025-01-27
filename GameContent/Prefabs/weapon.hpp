//
//
//
//


#include "Entity.hpp"
#include "EntityManager.hpp"
#include "Seedbank/AssetManager.hpp"

#include "Component.hpp"
#include "GameComponents.hpp"

#include <cstddef>
#include <memory>

namespace Prefab
{
    class Weapon
    {
        private:            
            void init(const Inst& inst);
            
        public:
            Weapon(const Inst& inst);
            
        
    };
    
}

