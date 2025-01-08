//
//  Scene.cpp
//  SaplingEngine, Canopy Scene Manager
//  

#include "Scene.hpp"


Scene::Scene(Engine& engine) : m_engine(engine)
{
    m_entityManager = std::make_shared<EntityManager>();
    m_input = std::make_shared<Input>();
}

void Scene::sRender(EntityList& entities)
{
    for (const auto& e : entities)
    {
        if (e->hasComponent<CSprite>())
        {
            
            auto& cSprite = e->getComponent<CSprite>();
            auto& cTransform = e->getComponent<CTransform>();

            if (cSprite.type == CSprite::Type::Animated)
            {
                // update the animation frame                
                if (m_engine.getCurrentFrame() % cSprite.animationSpeed == 0 )
                {
                    cSprite.currentFrame = (cSprite.currentFrame + 1) % cSprite.numFrames;
                }
                // m_engine.getWindow().draw_sprite(cSprite.texture, cTransform.position, cTransform.rotation, (int)cSprite.currentFrame);
                // return;
            }
            m_engine.getWindow().draw_sprite(cSprite.texture, cTransform.position, cTransform.rotation, (int)cSprite.currentFrame);
        }
    }
}

GameScene::GameScene(Engine& engine) : Scene(engine){
    init();
}

void GameScene::init(){
    /// Setup input actions
    m_input->makeAxis("moveX", SAPP_KEYCODE_D, SAPP_KEYCODE_A);
    m_input->makeAxis("moveY", SAPP_KEYCODE_S, SAPP_KEYCODE_W);
    m_input->makeAction("jump", {SAPP_KEYCODE_SPACE, 57});
    // m_input->makeAction("spawn", {sf::Keyboard::Scan::Scancode::R});

    
    /// Initial systems to run one time
    sSpawnPlayer();
    
    Debug::log("init game scene");
}

void Scene::enable() {
    // set the window's event callback to our input system
    m_engine.getWindow().SetEventCallback([this](const sapp_event* e) { m_input->update(e); });
}

void Scene::disable()
{
    Debug::log("disabling scene");
}

void GameScene::update(){
    //  Updates
    sSceneTime();
    m_entityManager->update();

    //  Systems
    sObstacleSpawner();
    sCollisionHandler(m_entityManager->getEntities("player").front(), m_entityManager->getEntities("obstacle"));
    sPlayerGravity(m_entityManager->getEntities("player").front());
    sPlayerController(m_entityManager->getEntities("player").front());

    //  Systems (ordered)
    sMove(m_entityManager->getEntities("dynamic"));
    // sRender(m_entityManager->getEntities());
    auto entitiesWithSprite = m_entityManager->getEntitiesByComponent<CSprite>();
    sRender(entitiesWithSprite);
    sDeleteOffScreen(m_entityManager->getEntities("obstacle"));

}

void GameScene::sSpawnPlayer() const {
    const auto e = m_entityManager->addEntity({"player", "dynamic"});
    e->addComponent<CTransform>(glm::vec2(-500, 100), glm::vec2(0, 0));
    e->addComponent<CSprite>(m_engine.getAssets()->getTexture("playerSheet"), 4);
    e->addComponent<CPlayerControls>(0.0f, 400.0f);
    e->addComponent<CBBox>(64, 64);
}

void GameScene::sPlayerGravity(const std::shared_ptr<Entity>& player) {
    const float GRAVITY = 900.0f;
    auto& transform = player->getComponent<CTransform>();
    auto& controls = player->getComponent<CPlayerControls>();
    if (transform.position.y >= 200){
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
    std::uniform_real_distribution<float> range(-250.0f, 250.0f);
    const float randomY = range(gen);
    
    const auto e = m_entityManager->addEntity({"obstacle", "dynamic"});
    e->addComponent<CTransform>(glm::vec2(640, randomY), glm::vec2(-100, 0));
    e->addComponent<CSprite>(m_engine.getAssets()->getTexture("obstacle"));
    e->addComponent<CBBox>(64, 64);
}

void GameScene::sDeleteOffScreen(const EntityList& entities){
    for (const auto& e : entities){
        if (e->getComponent<CTransform>().position.x < -640){
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