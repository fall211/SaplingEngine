//
//
//
//

#pragma once

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
            virtual void spawn(const Inst& inst) = 0;
        
    };
    
    class ForestSpirit : public Enemy
    {
        private:
            void init(const Inst& inst);
            
        public:
            ForestSpirit(const Inst& inst);
            void spawn(const Inst& inst) override;
        
    };
    
    class Hiisi : public Enemy
    {
        private:
            void init(const Inst& inst);
            
        public:
            Hiisi(const Inst& inst);
            void spawn(const Inst& inst) override;
    };
    
    class Serpent : public Enemy
    {
        private:
            void init(const Inst& inst);
            
        public:
            Serpent(const Inst& inst);
            void spawn(const Inst& inst) override;
    };
    
    class FrostWolf : public Enemy
    {
        private:
            void init(const Inst& inst);
            
        public:
            FrostWolf(const Inst& inst);
            void spawn(const Inst& inst) override;
    };
}

