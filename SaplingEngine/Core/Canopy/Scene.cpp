//
//  Scene.cpp
//  SaplingEngine, Canopy Scene Manager
//  

#include "Scene.hpp"
#include "Core/Debug.hpp"
#include <string>


Scene::Scene(Engine& engine) : m_engine(engine){
    m_entityManager = std::make_shared<EntityManager>();
    m_input = std::make_shared<Input>();
}

void Scene::sRender(EntityList& entities){
    for (const auto& e : entities){
        if (e->hasComponent<CSprite>()){
            e->getComponent<CSprite>().sprite.setPosition(e->getComponent<CTransform>().position.x, e->getComponent<CTransform>().position.y);
            m_engine.getWindow().draw(e->getComponent<CSprite>().sprite);
        }
        else if (e->hasComponent<CAnimatedSprite>()) {
            auto& animatedSprite = e->getComponent<CAnimatedSprite>();
            animatedSprite.sprite.setPosition(e->getComponent<CTransform>().position.x, e->getComponent<CTransform>().position.y);
            animatedSprite.setAnimationFrame(m_engine.simTime(), m_engine.deltaTime());
            m_engine.getWindow().draw(animatedSprite.sprite);
        }
        else if (e->hasComponent<CShape>()){
            e->getComponent<CShape>().rectShape.setPosition(e->getComponent<CTransform>().position.x, e->getComponent<CTransform>().position.y);
            m_engine.getWindow().draw(e->getComponent<CShape>().rectShape);
        }
    }
}

GameScene::GameScene(Engine& engine) : Scene(engine){
    init();
}

void GameScene::init(){
    /// Setup input actions
    m_input->makeAxis("moveX", sf::Keyboard::Scan::Scancode::D, sf::Keyboard::Scan::Scancode::A);
    m_input->makeAxis("moveY", sf::Keyboard::Scan::Scancode::S, sf::Keyboard::Scan::Scancode::W);
    m_input->makeAction("jump", {sf::Keyboard::Scan::Scancode::Space, 57});
    m_input->makeAction("spawn", {sf::Keyboard::Scan::Scancode::R});

    
    /// Initial systems to run one time
    sSpawnPlayer();
    
    Debug::log("init game scene");
}

void GameScene::update(){
    /// Updates
    sSceneTime();
    m_entityManager->update();
    m_input->update(m_engine.getWindow());
    
    /// Systems
    sObstacleSpawner();
    sCollisionHandler(m_entityManager->getEntities("player").front(), m_entityManager->getEntities("obstacle"));
    sPlayerGravity(m_entityManager->getEntities("player").front());
    sPlayerController(m_entityManager->getEntities("player").front());
    
    /// Systems (ordered)
    sMove(m_entityManager->getEntities("dynamic"));
    sRender(m_entityManager->getEntities());
    sDeleteOffScreen(m_entityManager->getEntities("obstacle"));

}

void GameScene::sSpawnPlayer() const {
    const auto e = m_entityManager->addEntity({"player", "dynamic"});
    e->addComponent<CTransform>(glm::vec2(static_cast<float>(m_engine.getWindow().getSize().x ) / 4 - 32, 100), glm::vec2(0, 0));
    // e->addComponent<CSprite>(m_engine.assets->getTexture("player"));
    e->addComponent<CAnimatedSprite>(m_engine.getAssets()->getTexture("playerSheet"));
    e->addComponent<CPlayerControls>(0.0f, 400.0f);
    e->addComponent<CBBox>(64, 64);
}

void GameScene::sPlayerGravity(const std::shared_ptr<Entity>& player) {
    const float GRAVITY = 900.0f;
    auto& transform = player->getComponent<CTransform>();
    auto& controls = player->getComponent<CPlayerControls>();
    if (transform.position.y >= 500){
        transform.velocity.y = 0;
        controls.grounded = true;
        return;
    }
    controls.grounded = false;
    transform.velocity.y += GRAVITY * m_engine.deltaTime();
}

void GameScene::sPlayerController(const std::shared_ptr<Entity>& player) const {
    const auto& controls = player->getComponent<CPlayerControls>();
    auto& transform = player->getComponent<CTransform>();
    if (m_input->isAction("jump") && controls.grounded) {
        transform.velocity.y -= static_cast<float>(controls.jumpStr);
    }
}

void GameScene::sMove(const EntityList& entities) {
    for (const auto& e : entities){
        if (e->hasComponent<CTransform>()){
            auto& transform = e->getComponent<CTransform>();
            transform.position += transform.velocity * m_engine.deltaTime();
        }
    }
}

void GameScene::sSceneTime(){
    m_obstacleSpawnTimer += m_engine.deltaTime();
}

void GameScene::sObstacleSpawner(){
    if (m_obstacleSpawnTimer < 1) return;
    m_obstacleSpawnTimer = 0;
    // Generate random position
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> range(200.0f, 500.0f);
    const float randomY = range(gen);
    
    const auto e = m_entityManager->addEntity({"obstacle", "dynamic"});
    e->addComponent<CTransform>(glm::vec2(1420, randomY), glm::vec2(-100.0f, 0));
    e->addComponent<CSprite>(m_engine.getAssets()->getTexture("obstacle"));
    e->addComponent<CBBox>(64, 64);
}

void GameScene::sDeleteOffScreen(const EntityList& entities){
    for (const auto& e : entities){
        if (e->getComponent<CTransform>().position.x < -100){
            e->destroy();
        }
    }
}

void GameScene::sCollisionHandler(const std::shared_ptr<Entity>& player, const EntityList& obstacles) {
    for (const auto& e : obstacles) {
        if (e->hasComponent<CBBox>()) { // ignore entities with no bounding box
            if (const glm::vec2 collision = Physics2D::bBoxCollision(player, e); collision != glm::vec2(0, 0)) {
                e->destroy();
            }
        }
    }
}


MenuScene::MenuScene(Engine& engine) : Scene(engine){
    init();
}

void MenuScene::init(){
    Debug::log("init menu scene");
    m_input->makeAction("sceneChange", {sf::Keyboard::Scan::Scancode::R});

}

void MenuScene::update(){
    m_entityManager->update();
    m_input->update(m_engine.getWindow());
    
    sTest();
    
    sRender(m_entityManager->getEntities());
}

void MenuScene::sTest() const {
    if (m_input->isActionDown("sceneChange")){
        m_engine.changeCurrentScene("game");
    }
}
