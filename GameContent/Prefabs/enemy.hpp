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
    class Enemy
    {
        private:            
            void init(const Inst& inst);
            
        public:
            Enemy(const Inst& inst);    
            void spawn(const Inst& inst);
        
            
        
    };
    
}

