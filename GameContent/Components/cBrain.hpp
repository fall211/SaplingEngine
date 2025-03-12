//
//
//
//

#pragma once

#include "Component.hpp"
#include "Entity.hpp"
#include "Debug.hpp"
#include "cDamage.hpp"
#include "Audio/AudioEngine.hpp"



namespace Comp
{
    class Brain : public Component 
    {
        public:
        
            enum class State
            {
                IDLE,
                PATROL,
                CHASE,
                PRE_ATTACK,
                ATTACK,
                FLEE,
                DIE
            };
        
            explicit Brain(Inst inst) 
            :   Component(std::move(inst))
            {}
            
            State state = State::IDLE;
            float detectRange = 100.0f;
            float attackRange = 50.0f;
            float attackCooldown = 2.0f;
            bool finishedLastAttack = true;
            float stopPursuitRange = 150.0f;
            float jumpCooldown = 1.0f;
            float jumpHeight = 250.0f;
            bool grounded = false;
            
            float speed = 55.0f;
            float dashSpeed = 300.0f;
            int moveDirection = 1;
            
            float _time = 0.0f;
            float lastJumpTime = 0.0f;
            float lastAttackTime = 0.0f;

            void Update(float dt)
            {
                _time += dt;
            }
            
            bool CanJump()
            {
                return _time - lastJumpTime >= jumpCooldown && grounded;
            }
            
            bool CanAttack()
            {
                return _time - lastAttackTime >= attackCooldown && grounded && finishedLastAttack;
            }
            
            void OnAddToEntity() override
            {
                inst->requestAddTag("brain");
                inst->ListenForEvent<Inst>("Jump", E_Jump);
                inst->ListenForEvent<Inst>("Attack", E_Attack);
                inst->ListenForEvent<Inst>("FinishAttack", E_FinishAttack);
                inst->ListenForEvent<Inst>("FloorCollision", E_FloorCollision);
                inst->ListenForEvent<Inst>("Death", E_Death);
            }
            
            void OnRemoveFromEntity() override
            {
                inst->requestRemoveTag("brain");
                inst->RemoveEventCallback("Jump", E_Jump);
                inst->RemoveEventCallback("Attack", E_Attack);
                inst->RemoveEventCallback("FinishAttack", E_FinishAttack);
                inst->RemoveEventCallback("FloorCollision", E_FloorCollision);
                inst->RemoveEventCallback("Death", E_Death);
            }
            
            
            static void Jump(const Inst& inst)
            {
                if (inst->hasComponent<Brain>() && inst->hasComponent<Transform>())
                {
                    auto& transform = inst->getComponent<Transform>();
                    auto& brain = inst->getComponent<Brain>();
                    transform.velocity.y = -brain.jumpHeight;
                    brain.lastJumpTime = brain._time;
                    brain.grounded = false;
                }
            }

            static void Attack(const Inst& inst)
            {
                if (inst->hasComponent<Brain>())
                {
                    auto& brain = inst->getComponent<Brain>();
                    brain.lastAttackTime = brain._time;
                    brain.finishedLastAttack = false;
                    
                    if (inst->hasComponent<Transform>())
                    {
                        auto& transform = inst->getComponent<Transform>();
                        transform.velocity.x = brain.dashSpeed * brain.moveDirection;
                        AudioEngine::getInstance()->playSound("dash");
                    }
                }
                if (inst->hasComponent<Damage>())
                {
                    auto& damage = inst->getComponent<Damage>();
                    damage.enabled = true;
                }
            }
            
            static void FinishAttack(const Inst& inst)
            {
                if (inst->hasComponent<Brain>())
                {
                    auto& brain = inst->getComponent<Brain>();
                    brain.finishedLastAttack = true;
                }
                if (inst->hasComponent<Damage>())
                {
                    auto& damage = inst->getComponent<Damage>();
                    damage.enabled = false;
                }
            }
            
            static void FloorCollision(const Inst& inst)
            {
                if (inst->hasComponent<Brain>())
                {
                    auto& brain = inst->getComponent<Brain>();
                    brain.grounded = true;
                }
            }
            
            static void Death(const Inst& inst)
            {
                inst->destroy();
            }
            
        private:
            std::function<void(Inst)> E_Jump = Brain::Jump;
            std::function<void(Inst)> E_Attack = Brain::Attack;
            std::function<void(Inst)> E_FinishAttack = Brain::FinishAttack;
            std::function<void(Inst)> E_FloorCollision = Brain::FloorCollision;
            std::function<void(Inst)> E_Death = Brain::Death;
            
    };
    
}