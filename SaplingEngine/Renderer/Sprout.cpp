//
//  Sprout.cpp
//  Sapling Engine, Sprout Renderer
//

#if defined(__APPLE__)
    #define SOKOL_METAL
#elif defined(_WIN32)
    #define SOKOL_D3D11
    #define SOKOL_IMPL
#else
    #define SOKOL_GLCORE
    #define SOKOL_IMPL
#endif

#define SOKOL_NO_ENTRY

#include "Renderer/Sprout.hpp"
#include "Utility/Color.hpp"
#include "Renderer/quad.h"

#include <algorithm>
#include <iostream>

namespace Sprout 
{
    glm::vec2 getPivotOffset(Pivot pivot) {
        switch (pivot) {
            case Pivot::TOP_LEFT:   return glm::vec2(0.0f, 0.0f);
            case Pivot::TOP_CENTER: return glm::vec2(0.5f, 0.0f);
            case Pivot::TOP_RIGHT:  return glm::vec2(1.0f, 0.0f);
            case Pivot::CENTER_LEFT:   return glm::vec2(0.0f, 0.5f);
            case Pivot::CENTER:        return glm::vec2(0.5f, 0.5f);
            case Pivot::CENTER_RIGHT:  return glm::vec2(1.0f, 0.5f);
            case Pivot::BOTTOM_LEFT:      return glm::vec2(0.0f, 1.0f);
            case Pivot::BOTTOM_CENTER:    return glm::vec2(0.5f, 1.0f);
            case Pivot::BOTTOM_RIGHT:     return glm::vec2(1.0f, 1.0f);
            default:                   return glm::vec2(0.5f, 0.5f); // default to center
        }
    }
    
    glm::vec2 getAnchorOffset(Pivot pivot)
    {
        switch (pivot) {
            case Pivot::TOP_LEFT:   return glm::vec2(0.0f, 0.0f);
            case Pivot::TOP_CENTER: return glm::vec2(0.5f, 0.0f);
            case Pivot::TOP_RIGHT:  return glm::vec2(1.0f, 0.0f);
            case Pivot::CENTER_LEFT:   return glm::vec2(0.0f, 0.5f);
            case Pivot::CENTER:        return glm::vec2(0.5f, 0.5f);
            case Pivot::CENTER_RIGHT:  return glm::vec2(1.0f, 0.5f);
            case Pivot::BOTTOM_LEFT:      return glm::vec2(0.0f, 1.0f);
            case Pivot::BOTTOM_CENTER:    return glm::vec2(0.5f, 1.0f);
            case Pivot::BOTTOM_RIGHT:     return glm::vec2(1.0f, 1.0f);
            default:                   return glm::vec2(0.0f, 0.0f); // default to top left
        }
    }
    
    
    Window::Window(int width, int height, const char* title)
        :   m_width(width), 
            m_height(height), 
            m_title(title)
        {    
            memset(&m_state, 0, sizeof(m_state));
            memset(&draw_frame, 0, sizeof(draw_frame));
    
            m_aspectRatio = static_cast<float>(m_width) / static_cast<float>(m_height);
            
            draw_frame.view_projection = glm::ortho(
                0.0f,
                1.0f * (m_width),
                1.0f * (m_height),
                0.0f,
                1.0f,
                -1.0f
            );
            
            draw_frame.orthoSize = glm::vec2(m_width, m_height);
            draw_frame.camera_xform = glm::mat4(1.0f);

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
        sg_desc desc = {};
        desc.environment = sglue_environment();   
        sg_setup(&desc);

        // vertex buffer 
        sg_buffer_desc vbuf_desc = {};
        vbuf_desc.size = sizeof(draw_frame.quads);
        vbuf_desc.usage = SG_USAGE_DYNAMIC;
        vbuf_desc.label = "quad-vertices";
        
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
        
        sg_buffer_desc ibuf_desc = {};
        ibuf_desc.type = SG_BUFFERTYPE_INDEXBUFFER;
        ibuf_desc.size = sizeof(indices);
        ibuf_desc.data = SG_RANGE(indices);
        ibuf_desc.label = "quad-indices";
        
        m_state.bind.index_buffer = sg_make_buffer(&ibuf_desc);
        
        sg_sampler_desc sampler_desc = {};
        sampler_desc.min_filter = SG_FILTER_NEAREST;
        sampler_desc.mag_filter = SG_FILTER_NEAREST;
        sampler_desc.mipmap_filter = SG_FILTER_NEAREST;
        sampler_desc.wrap_u = SG_WRAP_REPEAT;
        sampler_desc.wrap_v = SG_WRAP_REPEAT;
        
        m_state.bind.samplers[SMP_default_sampler] = sg_make_sampler(&sampler_desc);
        
        // pipeline
        sg_shader shd = sg_make_shader(quad_shader_desc(sg_query_backend()));
        
        sg_pipeline_desc pip_desc = {};
        pip_desc.shader = shd;
        pip_desc.index_type = SG_INDEXTYPE_UINT16;
        pip_desc.layout.attrs[ATTR_quad_position0].format = SG_VERTEXFORMAT_FLOAT4;
        pip_desc.layout.attrs[ATTR_quad_color0].format = SG_VERTEXFORMAT_FLOAT4;
        pip_desc.layout.attrs[ATTR_quad_uv0].format = SG_VERTEXFORMAT_FLOAT2;
        pip_desc.layout.attrs[ATTR_quad_color_override0].format = SG_VERTEXFORMAT_FLOAT4;
        pip_desc.layout.attrs[ATTR_quad_bytes0].format = SG_VERTEXFORMAT_UBYTE4N;
        pip_desc.label = "quad-pipeline";
        
        sg_blend_state blend_state = {};
        blend_state.enabled = true;
        blend_state.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
        blend_state.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
        blend_state.op_rgb = SG_BLENDOP_ADD;
        blend_state.src_factor_alpha = SG_BLENDFACTOR_ONE;
        blend_state.dst_factor_alpha = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
        blend_state.op_alpha = SG_BLENDOP_ADD;
        
        pip_desc.colors[0].blend = blend_state;
        m_state.pip = sg_make_pipeline(&pip_desc);
        
        sg_pass_action pass_action = {};
        pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
        pass_action.colors[0].clear_value = {1.0f, 1.0f, 1.0f, 1.0f};
        m_state.pass_action = pass_action;


        // bake images into atlas
        bake_atlas();
        init_fonts();
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
        
        m_state.bind.images[IMG_texture0] = m_atlas.img;
        m_state.bind.images[IMG_fontTex1] = m_fontAtlases[0].img;

        sg_update_buffer(m_state.bind.vertex_buffers[0], SG_RANGE(draw_frame.quads));


        sg_pass pass = {};
        pass.action = m_state.pass_action;
        pass.swapchain = sglue_swapchain();
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
                // m_width = e->window_width;
                // m_height = e->window_height;
                
                // const float referenceWidth = 640.0f;
                // const float referenceHeight = 360.0f;

                // scale to fit screen
                draw_frame.view_projection = glm::ortho(
                    0.0f,
                    static_cast<float>(m_width),
                    static_cast<float>(m_height),
                    0.0f,
                    1.0f,
                    -1.0f
                );
                draw_frame.orthoSize = glm::vec2(static_cast<float>(m_width), static_cast<float>(m_height));
            }
        }
    }
        
    auto Window::sokol_main() -> sapp_desc 
    {
        sapp_desc desc = {};
        desc.init_cb = init_cb;
        desc.frame_cb = frame_cb;
        desc.cleanup_cb = cleanup_cb;
        desc.event_cb = event_cb;
        desc.width = instance->m_width;
        desc.height = instance->m_height;
        desc.high_dpi = true;
        desc.window_title = instance->m_title;
        return desc;
    }
    
    Sprout::Window* Window::instance = nullptr;
    
    void Window::Run()
    {
        sapp_run(sokol_main());
    }


} // namespace Sprout