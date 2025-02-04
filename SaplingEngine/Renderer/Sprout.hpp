//
//  Sprout.hpp
//  Sapling Engine, Sprout Renderer
//

#pragma once
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


#include "string"
#include <cassert>
#include <memory>
#include <chrono>

#include "sokol/sokol_gfx.h"
#include "sokol/sokol_app.h"
#include "sokol/sokol_glue.h"

#include "Texture.hpp"



namespace Sprout 
{
    
    const int MAX_QUADS = 8192;
    //const int MAX_VERTS = MAX_QUADS * 4;
        
    
    struct State 
    {
        sg_pipeline pip;
        sg_bindings bind;
        sg_pass_action pass_action;
    };
    
    /*
        * Vertex struct for Sprout.
        * pos (vec4): position of the vertex.
        * color (vec4): color of the vertex.
        * uv (vec2): uv coordinates of the vertex.
        * color_override (vec4): color override of the vertex.
    */
    struct Vertex 
    {
        glm::vec4 pos;
        glm::vec4 color;
        glm::vec2 uv;
        glm::vec4 color_override;
    };
    
    /*
        * Quad struct for Sprout.
        * vertices (array<Vertex, 4>): the vertices of the quad.
    */
    struct Quad 
    {
        std::array<Vertex, 4> vertices;
    };
    
    /*
        * Atlas struct for Sprout.
        * width (int): width of the atlas.
        * height (int): height of the atlas.
        * img (sg_image): the image of the atlas.
    */
    struct Atlas 
    {
        int width, height;
        sg_image img;
    };
    
    /*
        * DrawFrame struct for Sprout.
        * quads (array<Quad, MAX_QUADS>): the quads to draw.
        * num_quads (int): the number of quads to draw.
        * view_projection (mat4): the view projection matrix.
        * camera_xform (mat4): the camera transform matrix.
    */
    struct DrawFrame
    {
        std::array<Quad, MAX_QUADS> quads;
        int num_quads = 0;
        glm::mat4 view_projection;
        glm::mat4 camera_xform;
    };
    
    /*
        * Pre-defined pivots for use with Sprout::getPivotOffset.
    */
    enum class Pivot {
        BOTTOM_LEFT,
        BOTTOM_CENTER,
        BOTTOM_RIGHT,
        CENTER_LEFT,
        CENTER,
        CENTER_RIGHT,
        TOP_LEFT,
        TOP_CENTER,
        TOP_RIGHT
    };
    
    glm::vec2 getPivotOffset(Pivot pivot);
    
    class Window
    {
        public: 
            Window(int width, int height, const char* title);
            ~Window();
            static Window* instance;
            DrawFrame draw_frame; 
        
            /*
                * Runs the window, called when everything is initialized
            */
            void Run();
            
            using UpdateFrameCallback = std::function<void(double)>;
            
            /*
                * Sets the update frame callback
                * @param cb The callback to set
            */
            void SetUpdateFrameCallback(UpdateFrameCallback cb);
            
            using EventCallback = std::function<void(const sapp_event*)>;
            
            /*
                * Sets the event callback
                * @param cb The callback to set
            */
            void SetEventCallback(EventCallback cb);
            
            static auto sokol_main() -> sapp_desc;
            
            /*
                * Adds a texture to the window
                * @param tex The texture to add
            */            
            void addTexture(std::shared_ptr<Sprout::Texture> tex);
        
            /*
                * Draws a sprite to the screen, called from game render code.
                * @param texture The texture to draw
                * @param position The position to draw the sprite
                * @param layer The layer to draw the sprite
                * @param rotation The rotation of the sprite
                * @param frameNumber The frame number of the sprite
                * @param color_override The color override of the sprite
                * @param scale The scale of the sprite
            */
            void draw_sprite(
                std::shared_ptr<Sprout::Texture> texture,
                glm::vec2 position, 
                glm::f32 layer,
                glm::f32 rotation = 0.0f,
                glm::i32 frameNumber = 1,
                glm::vec4 color_override = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
                glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
                Pivot pivot = Pivot::CENTER
            );
            
            // TODO: draw text
            void draw_text();
            
            /*
                * Transforms a screen position to a world position.
                * @param screenPos The screen position to transform
                * @return The world position
            */
            static glm::vec2 screenToWorld(glm::vec2 screenPos);
            
            /*
                * Moves the camera by the given delta.
                * @param deltaX The x delta to move the camera
                * @param deltaY The y delta to move the camera
            */
            void translateCamera(glm::f32 deltaX, glm::f32 deltaY);
            
            /*
                * Sets the camera position.
                * @param position The position to set the camera to
            */
            void setCameraPosition(glm::vec2 position);
            
            /*
                * Gets the camera position.
                * @return The camera position
            */
            glm::vec2 getCameraPosition();

        private:
            int m_width = 0;
            int m_height = 0;
            const char* m_title;
            
            State m_state;
            UpdateFrameCallback m_update_frame_callback;
            EventCallback m_event_callback;
            
            Atlas m_atlas;
            std::vector<std::shared_ptr<Sprout::Texture>> m_textures;
            void bake_atlas();
        
            std::chrono::time_point<std::chrono::system_clock> m_init_time = std::chrono::system_clock::now();
            std::chrono::time_point<std::chrono::system_clock> m_last_frame_time = std::chrono::system_clock::now();
            double m_delta_time = 0.0;
        
            static void init_cb();
            static void frame_cb();
            static void cleanup_cb();
            static void event_cb(const sapp_event* e);
            
            void Init();
            void Frame();
            void Cleanup();
            void Event(const sapp_event* e);
            
            int getWidth() const { return m_width; }
            int getHeight() const { return m_height; }
            
            // drawing things onto the screen
            void draw_test();

            
            void draw_rect_projected(
                glm::mat4 projection, 
                glm::vec2 size, 
                glm::f32 layer,
                glm::vec4 uv,
                glm::vec4 color_override,
                Pivot pivot
            );
            
            void draw_quad_projected(
                glm::mat4 projection, 
                std::array<glm::vec4, 4> positions,
                std::array<glm::vec4, 4> colors,
                std::array<glm::vec2, 4> uvs,
                std::array<glm::vec4, 4> color_overrides
            );
            
    };
        
} // namespace Sprout