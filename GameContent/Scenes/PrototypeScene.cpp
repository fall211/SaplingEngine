//
//
//
//

#include "PrototypeScene.hpp"
#include "Component.hpp"
#include "Debug.hpp"
#include "Input/Input.hpp"
#include "enemy.hpp"

#include <_strings.h>
#include <memory>




PrototypeScene::PrototypeScene(Engine& engine) : Scene(engine)
{
    init();
}

void PrototypeScene::init()
{
    sSpawn();
}

void PrototypeScene::update()
{
    // first frame only
    if (m_engine.getCurrentFrame() == 0)
    {
        auto player = m_entityManager->getEntities("player").front();
        auto& playerTransform = player->getComponent<Comp::Transform>();
        std::string playerPosStr = std::to_string(playerTransform.position.x) + ", " + std::to_string(playerTransform.position.y);
        
        m_engine.getWindow().setCameraPosition(playerTransform.position);
        
        glm::vec2 camPos = m_engine.getWindow().getCameraPosition();
        std::string camPosStr = std::to_string(camPos.x) + ", " + std::to_string(camPos.y);
    }
    
    
    int renderDistance = 750;
    glm::vec2 cameraPosition = m_engine.getWindow().getCameraPosition();
    auto entitiesInRange = m_entityManager->getEntitiesInRange(cameraPosition, renderDistance);
    
    //start of frame - run systems
    System::Gravity(entitiesInRange);
    System::EnemySpawner(m_entityManager->getEntities("spawner"), m_entityManager, m_engine.deltaTime());
    System::EnemyAI(m_entityManager, m_engine.deltaTime());
    System::WeaponUse(m_entityManager->getEntities("weapon"), m_entityManager, m_engine.deltaTime());
    System::BubbleVelDecay(m_entityManager->getEntities("projectile"));
    System::Lifetime(entitiesInRange, m_engine.deltaTime());
    
    
    // mid frame - move entities
    sMoveCamera(m_entityManager->getEntities("player").front());
    System::Move(m_entityManager, entitiesInRange, m_engine.deltaTime());
    System::PlayerController::Update(m_entityManager->getEntities("player").front(), m_engine.deltaTime());

    
    // end of frame - animate & render
    System::MathAnim(entitiesInRange, m_engine.getCurrentFrame() / 60.0f);
    sRender(entitiesInRange);
    
    // post frame - debug draws
    System::DrawTransforms(entitiesInRange);
    // System::DrawBoundingBoxes(entitiesInRange);

}

void PrototypeScene::sSpawn()
{
    // auto map = System::SpawnMap(std::string(ASSETS_PATH) + "map.csv", *m_entityManager);
    auto optimizedMap = System::SpawnCollisionOptimizedMap(std::string(ASSETS_PATH) + "map.csv", *m_entityManager);


    auto player = m_entityManager->instantiatePrefab<Prefab::Player>();

    // m_entityManager->instantiatePrefab<Prefab::Weapon>();
    // m_entityManager->instantiatePrefab<Prefab::Enemy>();
    
    // System::FoliageSpawner(m_entityManager);
    AudioEngine::getInstance()->playSound("woodpecker", 5.0);

}

void PrototypeScene::sMoveCamera(const std::shared_ptr<Entity>& player)
{
    if (player->hasComponent<Comp::Transform>())
        {
            size_t camera_y_offset = 15;
            auto& target_pos = player->getComponent<Comp::Transform>().position;
            glm::vec2 camera_pos = m_engine.getWindow().getCameraPosition();
            
            glm::vec2 target = glm::vec2(target_pos.x, target_pos.y - camera_y_offset);
            
            float distance = glm::distance(target, camera_pos);
                
            float camera_speed = 0.25f * distance;
            camera_speed = glm::clamp(camera_speed, 0.1f, 10.0f);
            
            glm::vec2 direction = glm::normalize(target - camera_pos);
            camera_pos += direction * camera_speed;
            
            if (distance < 0.1f) {
                camera_pos = target;
            }
            
            m_engine.getWindow().setCameraPosition(camera_pos);
        }
}
