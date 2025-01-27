//
//  Sprout.cpp
//  Sapling Engine, Sprout Renderer
//


#include "Debug.hpp"
#include <chrono>
#define SOKOL_IMPL
#define SOKOL_GLCORE
#define SOKOL_NO_ENTRY
#include "Sprout.hpp"
#include "quad.h"

#include <algorithm>

namespace Sprout 
{
    glm::vec2 getPivotOffset(Pivot pivot) {
        switch (pivot) {
            case Pivot::BOTTOM_LEFT:   return glm::vec2(0.0f, 0.0f);
            case Pivot::BOTTOM_CENTER: return glm::vec2(0.5f, 0.0f);
            case Pivot::BOTTOM_RIGHT:  return glm::vec2(1.0f, 0.0f);
            case Pivot::CENTER_LEFT:   return glm::vec2(0.0f, 0.5f);
            case Pivot::CENTER:        return glm::vec2(0.5f, 0.5f);
            case Pivot::CENTER_RIGHT:  return glm::vec2(1.0f, 0.5f);
            case Pivot::TOP_LEFT:      return glm::vec2(0.0f, 1.0f);
            case Pivot::TOP_CENTER:    return glm::vec2(0.5f, 1.0f);
            case Pivot::TOP_RIGHT:     return glm::vec2(1.0f, 1.0f);
            default:                   return glm::vec2(0.5f, 0.5f); // default to center
        }
    }
    
    
    Window::Window(int width, int height, const char* title)
        :   m_width(width), 
            m_height(height), 
            m_title(title)
        {    
            instance = this;
        }
    
    Window::~Window()
    {
        delete instance;
    }
    
    void Window::init_cb()
    {
        instance->Init();
    }
    
    void Window::frame_cb()
    {
        instance->Frame();
    }
    
    void Window::cleanup_cb()
    {
        instance->Cleanup();
    }
    
    void Window::event_cb(const sapp_event* e)
    {
        instance->Event(e);
    }
    
    
    void Window::Init()
    {
        draw_frame.view_projection = glm::ortho(
            -0.5f * (m_width),
            0.5f * (m_width),
            0.5f * (m_height),
            -0.5f * (m_height),
            1.0f,
            -1.0f
        );
        
        // we draw in a 640x360 space, so we need to scale the view projection to match the screen size
        draw_frame.view_projection = glm::scale(draw_frame.view_projection, glm::vec3(1.0f / 640.0f * m_width, 1.0f / 360.0f * m_height, 1.0f));
        
        draw_frame.camera_xform = glm::mat4(1.0f);
        
        sg_desc desc = 
        {
            .environment = sglue_environment()
        };
        
        sg_setup(&desc);
        
        // vertex buffer 
        sg_buffer_desc vbuf_desc = {
            .size = sizeof(draw_frame.quads),
            .usage = SG_USAGE_DYNAMIC,
            .label = "quad-vertices"
        };
        
        m_state.bind.vertex_buffers[0] = sg_make_buffer(&vbuf_desc);
        
        // index buffer
        const int INDEX_BUFFER_COUNT = MAX_QUADS * 6;
        std::array<glm::u16, INDEX_BUFFER_COUNT> indices;
        
        for (int i = 0; i < INDEX_BUFFER_COUNT;)
        {
            indices.at(i + 0) = ((i/6)*4 + 0);
    		indices.at(i + 1) = ((i/6)*4 + 1);
    		indices.at(i + 2) = ((i/6)*4 + 2);
    		indices.at(i + 3) = ((i/6)*4 + 0);
    		indices.at(i + 4) = ((i/6)*4 + 2);
    		indices.at(i + 5) = ((i/6)*4 + 3);
    		i += 6;
        }
        
        sg_buffer_desc ibuf_desc = {
            .type = SG_BUFFERTYPE_INDEXBUFFER,
            .size = sizeof(indices),
            .data = SG_RANGE(indices),
            .label = "quad-indices"
        };
        
        m_state.bind.index_buffer = sg_make_buffer(&ibuf_desc);
        
        sg_sampler_desc sampler_desc = {
            .min_filter = SG_FILTER_NEAREST,
            .mag_filter = SG_FILTER_NEAREST,
            .mipmap_filter = SG_FILTER_NEAREST,
            .wrap_u = SG_WRAP_REPEAT,
            .wrap_v = SG_WRAP_REPEAT,
        };
        
        m_state.bind.samplers[SMP_default_sampler] = sg_make_sampler(&sampler_desc);
        
        // pipeline
        sg_shader shd = sg_make_shader(quad_shader_desc(sg_query_backend()));
        
        sg_pipeline_desc pip_desc = {
            .shader = shd,
            .index_type = SG_INDEXTYPE_UINT16,
            .layout = {
                .attrs = {
                    [ATTR_quad_position0].format = SG_VERTEXFORMAT_FLOAT4,
                    [ATTR_quad_color0].format = SG_VERTEXFORMAT_FLOAT4,
                    [ATTR_quad_uv0].format = SG_VERTEXFORMAT_FLOAT2,
                    [ATTR_quad_color_override0].format = SG_VERTEXFORMAT_FLOAT4
                }
            },
            .label = "quad-pipeline"
        };
        
        sg_blend_state blend_state = {
            .enabled = true,
            .src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA,
            .dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .op_rgb = SG_BLENDOP_ADD,
            .src_factor_alpha = SG_BLENDFACTOR_ONE,
            .dst_factor_alpha = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .op_alpha = SG_BLENDOP_ADD
        };
        
        pip_desc.colors[0].blend = blend_state;
        
        m_state.pip = sg_make_pipeline(&pip_desc);
        
        m_state.pass_action = (sg_pass_action){
            .colors[0] = { .load_action=SG_LOADACTION_CLEAR, .clear_value = { 1.0f, 1.0f, 1.0f, 1.0f } }
        };
        
        // bake images into atlas
        Debug::log("baking atlas");
        bake_atlas();
    }
    
    bool sortByZ(const Quad& a, const Quad& b)
    {
        return a.vertices[0].pos.z < b.vertices[0].pos.z;
    }
    
    void Window::Frame()
    {
        // reset draw frame
        draw_frame.num_quads = 0;
        std::fill(draw_frame.quads.begin(), draw_frame.quads.end(), Quad{});
        
        // delta time calculation (not smoothed like sapp_frame_duration())
        // should be running at constant 60 fps, but just in case
        auto now = std::chrono::system_clock::now();
        m_delta_time = std::chrono::duration<double>(now - m_last_frame_time).count();
        m_last_frame_time = now;
        
        if (m_update_frame_callback) 
        {
            m_update_frame_callback(m_delta_time);
        }
        
        // sort quads by z so we have layers (z buffer alternative to keep transparency)
        std::sort(draw_frame.quads.begin(), draw_frame.quads.begin() + draw_frame.num_quads, sortByZ);
        
        m_state.bind.images[IMG_tex0] = m_atlas.img;
    
        sg_update_buffer(m_state.bind.vertex_buffers[0], SG_RANGE(draw_frame.quads));
        sg_pass pass = {
            .action = m_state.pass_action,
            .swapchain = sglue_swapchain()
        };
        sg_begin_pass(&pass);
        sg_apply_pipeline(m_state.pip);
        sg_apply_bindings(&m_state.bind);
        sg_draw(0, 6 * draw_frame.num_quads, 1);
        sg_end_pass();
        sg_commit();
    }
    
    void Window::Cleanup()
    {
        sg_shutdown();
    }
    
    void Window::Event(const sapp_event* e)
    {
        
        if (m_event_callback) 
        {
            m_event_callback(e);
        }
        // default behavior
        {
            if (e->type == SAPP_EVENTTYPE_KEY_DOWN) 
            {   
                if (e->key_code == SAPP_KEYCODE_ESCAPE) 
                {
                    sapp_request_quit();
                }
            }
            else if (e->type == SAPP_EVENTTYPE_RESIZED)
            {
                m_width = e->window_width;
                m_height = e->window_height;
                
                draw_frame.view_projection = glm::ortho(
                    -0.5f * (m_width),
                    0.5f * (m_width),
                    0.5f * (m_height),
                    -0.5f * (m_height),
                    1.0f,
                    -1.0f
                );
            }
        }
    }
    
    auto Window::sokol_main() -> sapp_desc 
    {
        return (sapp_desc)
        {
            .init_cb = init_cb,
            .frame_cb = frame_cb,
            .cleanup_cb = cleanup_cb,
            .event_cb = event_cb,
            .width = instance->m_width,
            .height = instance->m_height,
            .high_dpi = true,
            .window_title = instance->m_title,
        };
    }
    
    Sprout::Window* Window::instance = nullptr;
    
    void Window::Run()
    {
        sapp_run(sokol_main());
    }


} // namespace Sprout