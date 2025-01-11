//
//
//
//


#include "TestScene.hpp"


TestScene::TestScene(Engine& engine) : Scene(engine)
{
    init();
}

void TestScene::init()
{
    /// Setup input actions
    m_input->makeAxis("moveX", SAPP_KEYCODE_D, SAPP_KEYCODE_A);
    m_input->makeAxis("moveY", SAPP_KEYCODE_S, SAPP_KEYCODE_W);
    m_input->makeAction("jump", {SAPP_KEYCODE_SPACE});
    m_input->makeAction("lockCamera", {SAPP_KEYCODE_F});

    
    /// Initial systems to run one time
    sSpawnSquare();
    sSpawnPlayer();


    Debug::log("init game scene");
}


void TestScene::update()
{
    //  Updates
    sSceneTime();

    //  Systems
    sObstacleSpawner();
    sCollisionHandler(m_entityManager->getEntities("player").front(), m_entityManager->getEntities("obstacle"));
    sPlayerGravity(m_entityManager->getEntities("player").front());
    sPlayerController(m_entityManager->getEntities("player").front());
    sMoveSquare(m_entityManager->getEntities("square").front());
    sMoveCamera();
    
    //  Systems (ordered)
    sMove(m_entityManager->getEntities("dynamic"));
    
    auto entitiesWithSprite = m_entityManager->getEntitiesByComponent<Comp::Sprite>();
    sRender(entitiesWithSprite);
    sDeleteOffScreen(m_entityManager->getEntities("obstacle"));
}

void TestScene::sSpawnPlayer() const
{
    const auto e = m_entityManager->addEntity({"player", "dynamic", "cameraTarget"});
    e->addComponent<Comp::Transform>(glm::vec2(-500, 100), glm::vec2(0, 0));
    e->addComponent<Comp::Sprite>(m_engine.getAssets()->getTexture("playerSheet"), 4);
    e->getComponent<Comp::Sprite>().setLayer(Comp::Sprite::Layer::Foreground);
    e->addComponent<Comp::PlayerControls>(0.0f, 400.0f);
    e->addComponent<Comp::BBox>(64, 64);
}

void TestScene::sPlayerGravity(const std::shared_ptr<Entity>& player)
{
    const float GRAVITY = 900.0f;
    auto& transform = player->getComponent<Comp::Transform>();
    auto& controls = player->getComponent<Comp::PlayerControls>();
    if (transform.position.y >= 200){
        transform.velocity.y = 0;
        controls.grounded = true;
        return;
    }
    controls.grounded = false;
    transform.velocity.y += GRAVITY * m_engine.deltaTime();
}

void TestScene::sPlayerController(const std::shared_ptr<Entity>& player) const
{
    const auto& controls = player->getComponent<Comp::PlayerControls>();
    auto& transform = player->getComponent<Comp::Transform>();
    if (m_input->isAction("jump") && controls.grounded) {
        transform.velocity.y -= static_cast<float>(controls.jumpStr);
    }
}

void TestScene::sMove(const EntityList& entities)
{
    for (const auto& e : entities)
    {
        if (e->hasComponent<Comp::Transform>())
        {
            auto& transform = e->getComponent<Comp::Transform>();
            transform.position += transform.velocity * m_engine.deltaTime();
        }
    }
}

void TestScene::sSpawnSquare()
{
    const auto e = m_entityManager->addEntity({"square", "dynamic"});
    e->addComponent<Comp::Transform>(glm::vec2(0, 0), glm::vec2(0, 0));
    e->addComponent<Comp::Sprite>(m_engine.getAssets()->getTexture("test"));
    e->addComponent<Comp::BBox>(64, 64);
}

void TestScene::sMoveSquare(const std::shared_ptr<Entity>& square) const 
{
    auto& transform = square->getComponent<Comp::Transform>();
    transform.position = m_input->getMouseWorldPosition();
    
    if (m_input->getMouseUp(Input::MouseButton::LEFT))
    {
        if (square->hasComponent<Comp::Sprite>())
        {
            auto& cSprite = square->getComponent<Comp::Sprite>();
            // cycle through the layers
            cSprite.setLayer(static_cast<Comp::Sprite::Layer>((static_cast<int>(cSprite.layer) + 1) % static_cast<int>(Comp::Sprite::Layer::Count)));
            
            const std::string debugLayer = "Layer: " + std::to_string(static_cast<int>(cSprite.layer));
            Debug::log(debugLayer);
        }
    }
}

void TestScene::sMoveCamera()
{
    float deltaX = m_input->getAxis("moveX") * -200.0f * m_engine.deltaTime();
    float deltaY = m_input->getAxis("moveY") * -200.0f * m_engine.deltaTime();
    
    
    m_engine.getWindow().translateCamera(deltaX, deltaY);
    
    if (m_input->isActionUp("lockCamera"))
    {
        auto& target = m_entityManager->getEntities("cameraTarget").front()->getComponent<Comp::Transform>();
        m_engine.getWindow().setCameraPosition({target.position.x, target.position.y});
        
        Debug::log("camera moved");
    }
    
}

void TestScene::sSceneTime()
{
    m_obstacleSpawnTimer += m_engine.deltaTime();
}

void TestScene::sObstacleSpawner()
{    
    if (m_obstacleSpawnTimer < 1) return;
    m_obstacleSpawnTimer = 0;
    // Generate random position
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> range(-250.0f, 250.0f);
    const float randomY = range(gen);
    
    const auto e = m_entityManager->addEntity({"obstacle", "dynamic"});
    e->addComponent<Comp::Transform>(glm::vec2(640, randomY), glm::vec2(-100, 0));
    e->addComponent<Comp::Sprite>(m_engine.getAssets()->getTexture("obstacle"));
    e->addComponent<Comp::BBox>(64, 64);
}

void TestScene::sDeleteOffScreen(const EntityList& entities)
{
    for (const auto& e : entities)
    {
        if (e->getComponent<Comp::Transform>().position.x < -640)
        {
            e->destroy();
        }
    }
}

void TestScene::sCollisionHandler(const std::shared_ptr<Entity>& player, const EntityList& obstacles)
{
    for (const auto& e : obstacles)
    {
        if (e->hasComponent<Comp::BBox>()) // ignore entities with no bounding box
        { 
            if (const glm::vec2 collision = Physics2D::bBoxCollision(player, e); collision != glm::vec2(0, 0))
            {
                e->destroy();
            }
        }
    }
}