//
//
//
//

#pragma once

#include <utility>

#include "Component.hpp"
#include "Entity.hpp"
#include "Debug.hpp"

namespace Comp
{

    class Rotate : public Component 
    {
        public:
            float rotationSpeed = 0.0f;
            explicit Rotate(Inst inst, float speed) : Component(std::move(inst)), rotationSpeed(speed) {}
            
            void OnAddToEntity() override
            {
                Debug::log("Rotate component added to entity");
                inst->ListenForEvent("speedUp", SpeedUp);
                inst->ListenForEvent("speedDown", SpeedDown);

                // inst->PushEvent("speedUp");                
            }
            
            static void SpeedUp(const Inst& inst)
            {
                if (inst->hasComponent<Rotate>())
                {
                    inst->getComponent<Rotate>().rotationSpeed *= 2;
                }
            }
            static void SpeedDown(const Inst& inst)
            {
                if (inst->hasComponent<Rotate>())
                {
                    inst->getComponent<Rotate>().rotationSpeed /= 2;
                }
            }
            
            void OnRemoveFromEntity() override
            {
                inst->RemoveEventCallback("speedUp", SpeedUp);
                inst->RemoveEventCallback("speedDown", SpeedDown);
            }
    };

}