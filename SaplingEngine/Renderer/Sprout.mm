//
//  Sprout.cpp
//  Sapling Engine, Sprout Renderer
//


#define SOKOL_IMPL
#define SOKOL_GLCORE
#define SOKOL_NO_ENTRY
#include "Sprout.hpp"
#include "../Core/Debug.hpp"

#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb/stb_rect_pack.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

namespace Sprout {

Texture::~Texture() {
    //stbi_image_free(m_pixels);
}
    
bool Texture::loadFromFile(const std::string& path) {
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data) {
        // Debug::log(("Failed to load image: %s", path.c_str()));
        return false;
    }
        
    // stbi_image_free(data);
        
    m_pixels = data;
    m_width = width;
    m_height = height;
    
    registerTexture();
    
    return true;
}

void Texture::registerTexture() {
    Window::instance->addTexture(*this);
}

glm::i32 Texture::getWidth() {
    return m_width;
}

glm::i32 Texture::getHeight() {
    return m_height;
}

glm::vec4 Texture::getAtlasUVs() {
    return m_atlas_uvs;
}

void Texture::setAtlasUVs(glm::vec4& uvs) {
    m_atlas_uvs = uvs;
}

unsigned char* Texture::getPixels() {
    return m_pixels;
}

Window::Window(int width, int height, const char* title)
    : m_width(width), m_height(height), m_title(std::move(title)) {    
        instance = this;
}

Window::~Window() {
    delete instance;
}

void Window::init_cb() {
    instance->Init();
}
void Window::frame_cb() {
    instance->Frame();
}
void Window::cleanup_cb() {
    instance->Cleanup();
}
void Window::event_cb(const sapp_event* e) {
    instance->Event(e);
}

void Window::Init(){
    
    sg_desc desc = {
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
                [ATTR_quad_position].format = SG_VERTEXFORMAT_FLOAT2,
                [ATTR_quad_color0].format = SG_VERTEXFORMAT_FLOAT4,
                [ATTR_quad_uv0].format = SG_VERTEXFORMAT_FLOAT2,
                [ATTR_quad_bytes0].format = SG_VERTEXFORMAT_UBYTE4N,
                [ATTR_quad_color_override0].format = SG_VERTEXFORMAT_FLOAT4
            }
        },
        .label = "quad-pipeline"
    };
    
    m_state.pip = sg_make_pipeline(&pip_desc);
    
    m_state.pass_action = (sg_pass_action){
        .colors[0] = { .load_action=SG_LOADACTION_CLEAR, .clear_value = { 0.0f, 0.0f, 0.0f, 1.0f } }
    };
    
    // bake images into atlas
    Debug::log("baking atlas");
    bake_atlas();
    Debug::log("atlas baked");
    
    
}

void Window::bake_atlas() {
    
    m_atlas.width = 1024;
    m_atlas.height = 1024;
    
    // data
    unsigned char* atlas_data = (unsigned char*)malloc(m_atlas.width * m_atlas.height * 4);
    
    // clear atlas
    memset((void*)atlas_data, 0, m_atlas.width * m_atlas.height * 4);
    
    // all textures are in m_textures (std::vector<Texture>)
    // we need to rect_pack (stb) all textures into the atlas, 
    // and save the uv coordinates in the texture object
    // image data is unsigned char* and can be accessed with getPixels()
    // image width and height can be accessed with getWidth() and getHeight()
    
    stbrp_context context;
    stbrp_node* nodes = (stbrp_node*)malloc(sizeof(stbrp_node) * m_atlas.width);
    stbrp_init_target(&context, m_atlas.width, m_atlas.height, nodes, m_atlas.width);
    
    for (auto& tex : m_textures) {
        stbrp_rect rect;
        rect.id = 0;
        rect.w = tex.getWidth();
        rect.h = tex.getHeight();
        
        if (!stbrp_pack_rects(&context, &rect, 1)) {
            Debug::log("Failed to pack rect");
            continue;
        }
        
        // copy data to atlas
        for (int y = 0; y < tex.getHeight(); y++) {
            for (int x = 0; x < tex.getWidth(); x++) {
                int src_index = (y * tex.getWidth() + x) * 4;
                int dst_index = ((rect.y + y) * m_atlas.width + (rect.x + x)) * 4;
                
                atlas_data[dst_index + 0] = tex.getPixels()[src_index + 0];
                atlas_data[dst_index + 1] = tex.getPixels()[src_index + 1];
                atlas_data[dst_index + 2] = tex.getPixels()[src_index + 2];
                atlas_data[dst_index + 3] = tex.getPixels()[src_index + 3];
            }
        }
        
        // save uv coordinates
        glm::vec4 uv = glm::vec4(
            (float)rect.x / m_atlas.width,
            (float)rect.y / m_atlas.height,
            (float)(rect.x + rect.w) / m_atlas.width,
            (float)(rect.y + rect.h) / m_atlas.height
        );
        
        tex.setAtlasUVs(uv);    
        
    }
    
    
    // create image
    sg_image_desc img_desc = {
        .width = m_atlas.width,
        .height = m_atlas.height,
        .pixel_format = SG_PIXELFORMAT_RGBA8,
        .data.subimage[0][0] = {
            .ptr = atlas_data,
            .size = (size_t)m_atlas.width * m_atlas.height * 4
        }
    };
    
    Debug::log("loading image");
    m_atlas.img = sg_make_image(&img_desc);
    
    // write image to png
    stbi_write_png("atlas.png", m_atlas.width, m_atlas.height, 4, atlas_data, m_atlas.width * 4);
    
    // free data
    free(atlas_data);
    free(nodes);

}

void Window::addTexture(const Texture& tex) {
    m_textures.push_back(tex);
}

void Window::Frame() {
    // reset draw frame
    draw_frame.num_quads = 0;
    std::fill(draw_frame.quads.begin(), draw_frame.quads.end(), Quad{});
    
    
    //draw_test();
    
    if (m_update_frame_callback) {
        m_update_frame_callback(sapp_frame_duration());
    }

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

void Window::Cleanup() {
    sg_shutdown();
}

void Window::Event(const sapp_event* e) {
    if (e->type == SAPP_EVENTTYPE_KEY_DOWN) {
        if (e->key_code == SAPP_KEYCODE_ESCAPE) {
            sapp_request_quit();
        }
    }
}

auto Window::sokol_main(int argc, char* argv[], int width, int height, const char* title) -> sapp_desc {
    return (sapp_desc){
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

void Window::Run() {
    sapp_run(sokol_main(0, nullptr, m_width, m_height, m_title));
}

void Window::SetUpdateFrameCallback(UpdateFrameCallback callback) {
    m_update_frame_callback = std::move(callback);
    if (m_update_frame_callback) {
        Debug::log("frame update callback is set");
    }
}

} // namespace Sprout