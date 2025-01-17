//
//
//
//


#pragma once

#include "Component.hpp"
#include "Entity.hpp"
#include "Debug.hpp"

namespace Comp
{
    class Moves : public Component 
    {
        public:
        
            bool grounded = true;
            float moveSpeed = 200.0f;
            float jumpStr = 500.0f;
                

            explicit Moves(const std::shared_ptr<Entity>& inst) 
            :   Component(inst)
            {}
            
            void OnAddToEntity() override
            {
                // add tags
                inst->requestAddTag("canmove");
                
            }
            
            
            void OnRemoveFromEntity() override
            {
                // remove tags
                inst->requestRemoveTag("canmove");
                
            }
            
            
            
            static void OnMove(Inst inst)
            {
                if (inst->hasComponent<Moves>() && inst -> hasComponent<Transform>())
                {
                    auto& move = inst->getComponent<Moves>();
                    auto& transform = inst->getComponent<Transform>();
                    
                    
                    
                }
            }
            
    };
}