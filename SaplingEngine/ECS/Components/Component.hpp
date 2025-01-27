//
//  Component.hpp
//  SaplingEngine
//

#pragma once


#include "Renderer/Texture.hpp"
#include "Sprout.hpp"
#include "glm/glm.hpp"
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

class Entity;

namespace Comp
{
    typedef std::shared_ptr<Entity> Inst;
    
    class Component{
    protected:
        Inst inst;
        
    public:
        bool has = false;
        Component(Inst inst) : inst(std::move(inst)) {};
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
            glm::vec3 scale = glm::vec3(1, 1, 1);
            Sprout::Pivot pivot = Sprout::Pivot::CENTER;
    
            Transform(Inst inst, const glm::vec2& positionin, const glm::vec2& velocityin);
            Transform(Inst inst, const glm::vec2& positionin);
    };
    
    
    /*
        * A bounding box component for collision detection.
        * w (f32): The width of the bounding box.
        * h (f32): The height of the bounding box.
    */
    class BBox final : public Component
    {
        public:
            float h;
            float w;
            bool isTrigger = false;
            bool isStatic = true;
            bool interactWithTriggers = false;
            std::vector<BBox*> collidingWith = {};
            
            BBox(Inst inst, float win, float hin);
            void OnAddToEntity() override;
            void OnRemoveFromEntity() override;
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
            void OnAddToEntity() override;
            void OnRemoveFromEntity() override;
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
            void OnAddToEntity() override;
            void OnRemoveFromEntity() override;
            
            
            enum class Layer {
                Background,
                Midground,
                Foreground,
                UserInterface,
                Count
            };
            
            void setLayer(Layer layerIn) { layer = layerIn; }
            
            void flipX(bool flip);
            
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
            glm::vec4 color_override = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            bool flip_X = false;
    };
    
    class FollowMouse final : public Component {};
    
    class SimplePlayerControls final : public Component 
    {
        public:
            float moveSpeed = 0.0f;
            int jumpStr;
            bool grounded = true;
            SimplePlayerControls(Inst inst, float speedIn, int jumpStrIn);
    };

    
    class TransformHierarchy final : public Component
    {
        public:
            Inst parent = nullptr;
            std::vector<Inst> children = {};
            
            TransformHierarchy(Inst inst);
            
            void setParent(Inst parentIn);
            void removeParent();
            
            void addChild(const Inst& child);
            void removeChild(const Inst& child);
    };
}