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

#include "sokol/sokol_gfx.h"
#include "sokol/sokol_app.h"
#include "sokol/sokol_glue.h"

#include "quad.h"
#include "../Core/Debug.hpp"



namespace Sprout {

const int MAX_QUADS = 8192;
const int MAX_VERTS = MAX_QUADS * 4;
    

struct State {
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
};

struct Vertex {
    glm::vec2 pos;
    glm::vec4 color;
    glm::vec2 uv;
    glm::vec4 color_override;
};

struct Quad {
    std::array<Vertex, 4> vertices;
};

struct Atlas {
    int width, height;
    sg_image img;
};

struct DrawFrame {
    std::array<Quad, MAX_QUADS> quads;
    int num_quads = 0;
    glm::mat4 view_projection;
    glm::mat4 camera_xform;
};

class Texture {
public:
    Texture() = default;
    ~Texture();
    
    auto loadFromFile(const std::string& path) -> bool;
    auto getPixels() -> unsigned char*;
    auto getWidth() -> glm::i32;
    auto getHeight() -> glm::i32;
    auto getAtlasUVs() -> glm::vec4;
    auto setAtlasUVs(glm::vec4 uvs) -> void;
    
    auto registerTexture() -> void;
    
private:
    glm::i32 m_width, m_height;
    glm::vec4 m_atlas_uvs = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    unsigned char* m_pixels;
};

class Window {
public: 
    Window(int width, int height, const char* title);
    ~Window();
    static Window* instance;
    DrawFrame draw_frame; 

    void Run();
    
    using UpdateFrameCallback = std::function<void(double)>;
    void SetUpdateFrameCallback(UpdateFrameCallback cb);
    
    using EventCallback = std::function<void(const sapp_event*)>;
    void SetEventCallback(EventCallback cb);
    
    static auto sokol_main(int argc, char* argv[], int width, int height, const char* title) -> sapp_desc;

    // void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    
    // texture stuff
    void addTexture(std::shared_ptr<Texture> tex);

    // this is what will be called from game code
    void draw_sprite(
        const std::shared_ptr<Sprout::Texture> texture,
        const glm::vec2 position, 
        const glm::vec4 color_override = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
        const glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
    );
    
    // TODO: draw text
    void draw_text();
    
private:
    int m_width = 0;
    int m_height = 0;
    const char* m_title;
    
    State m_state;
    UpdateFrameCallback m_update_frame_callback;
    EventCallback m_event_callback;
    
    Atlas m_atlas;
    std::vector<std::shared_ptr<Texture>> m_textures;
    void bake_atlas();


    static void init_cb();
    static void frame_cb();
    static void cleanup_cb();
    static void event_cb(const sapp_event* e);
    
    void Init();
    void Frame();
    void Cleanup();
    void Event(const sapp_event* e);
    
    
    // drawing things onto the screen
    void draw_test();
    
    void draw_rect_projected(
        glm::mat4 projection, 
        glm::vec2 size, 
        glm::vec4 uv,
        glm::vec4 color_override = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    
    void draw_quad_projected(
        glm::mat4 projection, 
        std::array<glm::vec2, 4> positions,
        std::array<glm::vec4, 4> colors,
        std::array<glm::vec2, 4> uvs,
        std::array<glm::vec4, 4> color_overrides
    );
    
};
    
} // namespace Sprout