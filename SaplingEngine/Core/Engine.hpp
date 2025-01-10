//
//  Engine.hpp
//  SaplingEngine
//

#pragma once

#include "Renderer/Sprout.hpp"
#include "Core/Canopy/Scene.hpp"
#include "Core/Seedbank/Assets.hpp"

#include <memory>
#include <string>


class Scene;
class Assets;

using namespace std::chrono;

using sceneMap = std::unordered_map<std::string, std::shared_ptr<Scene>>;

class Engine {
    Sprout::Window m_window = Sprout::Window(640 * 2, 360 * 2, "Sapling Engine");
    float m_deltaTime = 0.0f;
    float m_simTime = 0.0f;

    sceneMap m_scenes;
    std::shared_ptr<Scene> m_currentScene = nullptr;
    

    size_t m_currentFrame = 0;
    
    std::shared_ptr<Assets> m_assets;

public:

    Engine();
    ~Engine() = default;
    
    /**
     * The main loop of the engine. Started as soon as the program starts.
     */
    void main();
    
    /**
     * The main update function of the game, called each frame. Must be set as callback to the window.
    */
    void update(double dt);
    
    /**
     * Gets the current frame of the simulation.
     *
     * @return The current frame.
     */
    auto getCurrentFrame() const -> size_t { return m_currentFrame; }

    /**
     * Gets the delta time of the simaltion.
     * @return delta time
     */
    auto deltaTime() const -> float { return m_deltaTime; }

    /**
     * \brief Total time the simulation has been running.
     * \return total time of the simlation
     */
    auto simTime() const -> float { return m_simTime; }

    /**
     * Gets the Sprout window.
     *
     * @return The Sorout window.
     */
    auto getWindow() -> Sprout::Window& { return m_window; }


    /**
     * Makes a scene and adds it to the engine simulation.
     *
     * @param name  The name of the new scene
     * @param ptr  Pointer to the scene
     */
    void makeScene(const std::string& name, std::shared_ptr<Scene> ptr);
    
    /**
     * Adds a scene to the engine simulation.
     *
     * @tparam SceneType  The type of the scene
     * @param name  The name of the scene
     */
    template <typename SceneType>
    void newScene(const std::string& name)
    {
        makeScene(name, std::make_shared<SceneType>(*this));
    }
    
    void changeScene(const std::string& name);
    auto getScene(const std::string& name) -> std::shared_ptr<Scene>;
    auto getCurrentScene() -> std::shared_ptr<Scene>&;

    auto getAssets() const -> std::shared_ptr<Assets>;
    void setAssets(const std::shared_ptr<Assets>& newAssets);
    
};



