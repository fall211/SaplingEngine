//
//  Scene.cpp
//  SaplingEngine, Canopy Scene Manager
//  

#include "Scene.hpp"
#include "ECS/Components/Component.hpp"


Scene::Scene(Engine& engine) : m_engine(engine)
{
    m_entityManager = std::make_shared<EntityManager>();
    m_input = std::make_shared<Input>();
}

void Scene::enable() {
    // set the window's event callback to our input system
    m_engine.getWindow().SetEventCallback([this](const sapp_event* e) { m_input->update(e); });
}

void Scene::disable()
{
    Debug::log("disabling scene");
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
            glm::f32 layer = static_cast<glm::f32>(cSprite.layer) / static_cast<glm::f32>(CSprite::Layer::Count);
            m_engine.getWindow().draw_sprite(cSprite.texture, cTransform.position, layer, cTransform.rotation, (int)cSprite.currentFrame);
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

    
    /// Initial systems to run one time
    sSpawnSquare();
    sSpawnPlayer();


    Debug::log("init game scene");
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
    sMoveSquare(m_entityManager->getEntities("square").front());
    
    
    //  Systems (ordered)
    sMove(m_entityManager->getEntities("dynamic"));
    
    auto entitiesWithSprite = m_entityManager->getEntitiesByComponent<CSprite>();
    sRender(entitiesWithSprite);
    sDeleteOffScreen(m_entityManager->getEntities("obstacle"));
    
    // clean input to avoid double inputs
    m_input->clean();
}

void GameScene::sSpawnPlayer() const {
    const auto e = m_entityManager->addEntity({"player", "dynamic"});
    e->addComponent<CTransform>(glm::vec2(-500, 100), glm::vec2(0, 0));
    e->addComponent<CSprite>(m_engine.getAssets()->getTexture("playerSheet"), 4);
    e->getComponent<CSprite>().setLayer(CSprite::Layer::Foreground);
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

void GameScene::sSpawnSquare(){
    const auto e = m_entityManager->addEntity({"square", "dynamic"});
    e->addComponent<CTransform>(glm::vec2(0, 0), glm::vec2(0, 0));
    e->addComponent<CSprite>(m_engine.getAssets()->getTexture("test"));
    e->addComponent<CBBox>(64, 64);
}

void GameScene::sMoveSquare(const std::shared_ptr<Entity>& square) const {
    auto& transform = square->getComponent<CTransform>();
    transform.position = m_input->getMousePosition() - glm::vec2(720, 360);
    
    if (m_input->getMouseUp(Input::MouseButton::LEFT))
    {
        if (square->hasComponent<CSprite>())
        {
            auto& cSprite = square->getComponent<CSprite>();
            // cycle through the layers
            cSprite.setLayer(static_cast<CSprite::Layer>((static_cast<int>(cSprite.layer) + 1) % static_cast<int>(CSprite::Layer::Count)));
            
            const std::string debugLayer = "Layer: " + std::to_string(static_cast<int>(cSprite.layer));
            Debug::log(debugLayer);
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