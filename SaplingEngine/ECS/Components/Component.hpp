//
//  Component.hpp
//  SaplingEngine
//

#pragma once


#include "Renderer/Texture.hpp"
#include "glm/glm.hpp"
#include <memory>

namespace Comp
{
    
    class Component{
    public:
        bool has = false;
        Component()= default;
        virtual ~Component()= default;
    };
    
    class Transform final : public Component{
        public:
            glm::vec2 position = glm::vec2(0, 0);
            glm::vec2 velocity = glm::vec2(0, 0);
            glm::f32 rotation = 0.0f;
    
            Transform(const glm::vec2& positionin, const glm::vec2& velocityin);
    };
    
    
    class Lifetime final : public Component {
    public:
        float lifetime = 1.0f;
        explicit Lifetime(float lifetimein);
    };
    
    class BBox final : public Component {
    public:
        float w = 1.0f;
        float h = 1.0f;
        BBox(float win, float hin);
    };
    
    class BCircle final : public Component {
    public: 
        float radius = 1.0f;
        BCircle(float radiusIn);
    };
        
    class Sprite final : public Component {
    public:
        std::shared_ptr<Sprout::Texture> texture;
        explicit Sprite(const std::shared_ptr<Sprout::Texture>& texin);
        explicit Sprite(const std::shared_ptr<Sprout::Texture>& texin, float animSpeed);
        
        enum class Layer {
            Background,
            Midground,
            Foreground,
            UserInterface,
            Count
        };
        
        void setLayer(Layer layerIn) { layer = layerIn; }
        
        enum class Type {
            Static,
            Animated
        };
        
        Type type = Type::Static;
        Layer layer = Layer::Midground;
        
        // animated frame stuff
        size_t numFrames = 1;
        size_t currentFrame = 0;
        size_t frameSize;
        size_t animationSpeed = 60;
    };
    
    class FollowMouse final : public Component {
    };
    
    class PlayerControls final : public Component {
    public:
        float moveSpeed = 0.0f;
        int jumpStr;
        bool grounded = true;
        PlayerControls(float speedIn, int jumpStrIn);
    };

}