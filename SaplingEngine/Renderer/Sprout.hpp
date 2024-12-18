//
//  Sprout.hpp
//  Sapling Engine, Sprout Renderer
//

#pragma once
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
};

struct Quad {
    std::array<Vertex, 4> vertices;
};

static struct DrawFrame {
    std::array<Quad, MAX_QUADS> quads;
    int num_quads = 0;
    
    glm::mat4 view_projection;
    glm::mat4 camera_xform;
} draw_frame;

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

    static void init_cb();
    static void frame_cb();
    static void cleanup_cb();
    static void event_cb(const sapp_event* e);
    
    void Init();
    void Frame();
    void Cleanup();
    void Event(const sapp_event* e);
};
    
} // namespace Sprout