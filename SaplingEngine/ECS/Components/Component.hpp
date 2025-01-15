//
//  Component.hpp
//  SaplingEngine
//

#pragma once


#include "Renderer/Texture.hpp"
#include "glm/glm.hpp"
#include <cstddef>
#include <memory>
class Entity;

namespace Comp
{
    typedef const std::shared_ptr<Entity>& Inst;
    
    class Component{
    protected:
        Inst inst;
        
    public:
        bool has = false;
        Component(Inst inst) : inst(inst) {};
        ~Component()= default;
        
        virtual void OnAddToEntity() {}
        virtual void OnRemoveFromEntity() {}
    };
    
    /*
        * A transform component for position and velocity.
        * position (vec2): The position of the entity.
        * velocity (vec2): The velocity of the entity.
        * rotation (f32): The rotation of the entity.
    */
    class Transform final : public Component
    {
        public:
            glm::vec2 position = glm::vec2(0, 0);
            glm::vec2 velocity = glm::vec2(0, 0);
            glm::f32 rotation = 0.0f;
    
            Transform(Inst inst, const glm::vec2& positionin, const glm::vec2& velocityin);
    };
    
    class Lifetime final : public Component 
    {
        public:
            float lifetime = 1.0f;
            explicit Lifetime(Inst inst, float lifetimein);
    };
    
    /*
        * A bounding box component for collision detection.
        * w (f32): The width of the bounding box.
        * h (f32): The height of the bounding box.
    */
    class BBox final : public Component
    {
        public:
            float w = 1.0f;
            float h = 1.0f;
            BBox(Inst inst, float win, float hin);
    };
    
    /*
        * A bounding circle component for collision detection.
        * radius (f32): The radius of the bounding circle.
    */
    class BCircle final : public Component
    {
        public: 
            float radius = 1.0f;
            BCircle(Inst inst, float radiusIn);
    };
        
    
    /*
        * A sprite component for rendering textures.
        * Supports static and animated sprites as well as layering.
        * texture (shared_ptr<Texture>): The texture to render.
        * type (Type): The type of sprite (static or animated).
        * layer (Layer): The layer to render the sprite on.
        * numFrames (size_t): The number of frames in the animation.
        * currentFrame (size_t): The current frame of the animation.
        * frameSize (size_t): The size of each frame in the animation.
        * animationSpeed (size_t): The speed of the animation.
    */
    class Sprite final : public Component 
    {
        public:
            std::shared_ptr<Sprout::Texture> texture;
            explicit Sprite(Inst inst, const std::shared_ptr<Sprout::Texture>& texin);
            explicit Sprite(Inst inst, const std::shared_ptr<Sprout::Texture>& texin, float animSpeed);
            
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
    
    class FollowMouse final : public Component {};
    
    class PlayerControls final : public Component 
    {
        public:
            float moveSpeed = 0.0f;
            int jumpStr;
            bool grounded = true;
            PlayerControls(Inst inst, float speedIn, int jumpStrIn);
    };

}