//
//  Sprout.hpp
//  Sapling Engine, Sprout Renderer
//

#pragma once
#include "glm/fwd.hpp"
#include <functional>
#pragma clang diagnostic ignored "-Wunused-variable"




#include "glm/glm.hpp"
#include "string"

#include "sokol/sokol_gfx.h"
#include "sokol/sokol_app.h"
#include "sokol/sokol_glue.h"

#include "quad.h"

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
    glm::u8 tex_index;
    std::array<glm::u8, 3> pad;
    glm::vec4 color_override;
};

struct Quad {
    std::array<Vertex, 4> vertices;
};

struct Atlas {
    int width, height;
    sg_image img;
};

static struct DrawFrame {
    std::array<Quad, MAX_QUADS> quads;
    int num_quads = 0;
    
    glm::mat4 view_projection;
    glm::mat4 camera_xform;
} draw_frame;

struct Image {
    glm::i32 width, height;
    glm::vec4 atlas_uvs;
    unsigned char* pixels;
};

class Texture {
public:
    Texture() = default;
    ~Texture();
    
    auto loadFromFile(const std::string& path) -> bool;
    auto getImage() -> Image&;
    
private:
    Image m_image;
};

class Window {
public: 
    Window(int width, int height, const char* title);
    ~Window();

    void Run();
    
    using UpdateFrameCallback = std::function<void(double)>;
    void SetUpdateFrameCallback(UpdateFrameCallback cb);
    
    static auto sokol_main(int argc, char* argv[], int width, int height, const char* title) -> sapp_desc;

    // void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    

private:
    int m_width = 0;
    int m_height = 0;
    const char* m_title;
    
    static Window* m_instance;
    
    State m_state;
    UpdateFrameCallback m_update_frame_callback;
    Atlas m_atlas;

    static void init_cb();
    static void frame_cb();
    static void cleanup_cb();
    static void event_cb(const sapp_event* e);
    
    void Init();
    void Frame();
    void Cleanup();
    void Event(const sapp_event* e);
    
    void bake_atlas();
    
    
    // drawing things onto the screen
    
    void draw_test();
    
    void draw(
        Sprout::Texture& texture,
        glm::vec2 position, 
        glm::vec4 color_override = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)
    );
    
    void draw_rect_projected(
        glm::mat4 projection, 
        glm::vec2 size, 
        glm::vec4 color,
        glm::vec4 uv,
        glm::vec4 color_override
    );
    
    void draw_quad_projected(
        glm::mat4 projection, 
        std::array<glm::vec2, 4> positions,
        std::array<glm::vec4, 4> colors,
        std::array<glm::vec2, 4> uvs,
        std::array<glm::u8, 4> tex_indices,
        std::array<glm::vec4, 4> color_overrides
    );
    
};
    
} // namespace Sprout