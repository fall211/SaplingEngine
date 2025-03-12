//
//
//
//


#pragma once

#include "Component.hpp"
#include "cPickup.hpp"
#include "Entity.hpp"
#include "Debug.hpp"

#include <functional>
#include <utility>

namespace Comp
{
    class PickupHandler : public Component 
    {
        
        public:
            std::vector<Inst> pickupCandidates = {};
            bool holdingPickup = false;
        
            explicit PickupHandler(Inst inst) 
            :   Component(std::move(inst))
            {}
            
            void OnAddToEntity() override
            {
                inst->ListenForEvent<Inst>("PickupInput", E_PickupInput);
                inst->ListenForEvent<Inst>("ForceDrop", E_ForceDrop);
                inst->ListenForEvent<Inst, Inst>("TriggerEnter", E_TriggerEnter);
                inst->ListenForEvent<Inst, Inst>("TriggerExit", E_TriggerExit);
            }
            
            void OnRemoveFromEntity() override
            {
                inst->RemoveEventCallback("PickupInput", E_PickupInput);
                inst->RemoveEventCallback("ForceDrop", E_ForceDrop);
                inst->RemoveEventCallback("TriggerEnter", E_TriggerEnter);
                inst->RemoveEventCallback("TriggerExit", E_TriggerExit);
                
            }

            static void PickupInput(const Inst& inst)
            {
                if (inst->hasComponent<TransformHierarchy>())
                {
                    auto transform = inst->getComponent<TransformHierarchy>();
                    for (auto& child : transform.children)
                    {
                        if (child->hasComponent<Comp::Pickup>())
                        {
                            child->PushEvent("Drop", child);
                        }
                        return;
                    }
                }
                if (inst->hasComponent<PickupHandler>() && !inst->getComponent<PickupHandler>().pickupCandidates.empty())
                {
                    for (auto& pickup : inst->getComponent<PickupHandler>().pickupCandidates)
                    {
                        if (pickup->hasComponent<Comp::Pickup>()) 
                        {
                            pickup->PushEvent("Pickup", pickup, inst);
                        }
                    }
                }
            }
            
            static void ForceDrop(const Inst& inst)
            {
                if (inst->hasComponent<TransformHierarchy>())
                {
                    auto transform = inst->getComponent<TransformHierarchy>();
                    for (auto& child : transform.children)
                    {
                        if (child->hasComponent<Comp::Pickup>())
                        {
                            child->PushEvent("Drop", child);
                        }
                    }
                }
            }
            
            static void TriggerEnter(const Inst& inst, const Inst& other)
            {
                if (inst->hasComponent<TransformHierarchy>()
                    && inst->hasComponent<PickupHandler>()
                    && other->hasComponent<TransformHierarchy>()
                    && other->hasComponent<Comp::Pickup>())
                {
                    auto& pickupHandler = inst->getComponent<PickupHandler>();
                    auto& otherTransform = other->getComponent<TransformHierarchy>();
                    
                    if (otherTransform.parent == nullptr)
                    {
                        pickupHandler.pickupCandidates.push_back(other);
                    }
                    
                }
            }
            
            static void TriggerExit(const Inst& inst, const Inst& other)
            {
                if (inst->hasComponent<TransformHierarchy>()
                    && inst->hasComponent<PickupHandler>()
                    && other->hasComponent<TransformHierarchy>()
                    && other->hasComponent<Comp::Pickup>())
                {
                    auto& pickupHandler = inst->getComponent<PickupHandler>();
                    auto& otherTransform = other->getComponent<TransformHierarchy>();
                    
                    if (otherTransform.parent == nullptr)
                    {
                        pickupHandler.pickupCandidates.erase(std::remove(pickupHandler.pickupCandidates.begin(), pickupHandler.pickupCandidates.end(), other), pickupHandler.pickupCandidates.end());
                    }
                }
            }
            
        private:
        
            //events
            std::function<void(Inst)> E_PickupInput = PickupHandler::PickupInput;
            std::function<void(Inst)> E_ForceDrop = PickupHandler::ForceDrop;
            std::function<void(Inst, Inst)> E_TriggerEnter = PickupHandler::TriggerEnter;
            std::function<void(Inst, Inst)> E_TriggerExit = PickupHandler::TriggerExit;
    };
}