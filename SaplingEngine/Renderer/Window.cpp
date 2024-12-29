//
//  Window.cpp
//  SaplingEngine, Sprout Renderer
//

//  This file contains all the window functionality that is 
//  related to windowing but not part of sokol implementation.
//  For the sokol window setup, see Sprout.mm

#include "Core/Debug.hpp"
#include "Sprout.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb/stb_rect_pack.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

namespace Sprout {

    void Window::bake_atlas() {
        
        m_atlas.width = 256;
        m_atlas.height = 256;
        
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
            rect.w = tex->getWidth();
            rect.h = tex->getHeight();
            
            if (!stbrp_pack_rects(&context, &rect, 1)) {
                Debug::log("Failed to pack rect");
                continue;
            }
            
            // copy data to atlas
            for (int y = 0; y < tex->getHeight(); y++) {
                for (int x = 0; x < tex->getWidth(); x++) {
                    int src_index = (y * tex->getWidth() + x) * 4;
                    int dst_index = ((rect.y + y) * m_atlas.width + (rect.x + x)) * 4;
                    
                    atlas_data[dst_index + 0] = tex->getPixels()[src_index + 0];
                    atlas_data[dst_index + 1] = tex->getPixels()[src_index + 1];
                    atlas_data[dst_index + 2] = tex->getPixels()[src_index + 2];
                    atlas_data[dst_index + 3] = tex->getPixels()[src_index + 3];
                }
            }
            
            // save uv coordinates
            glm::vec4 uv = glm::vec4(
                (float)rect.x / m_atlas.width,
                (float)rect.y / m_atlas.height,
                (float)(rect.x + rect.w) / m_atlas.width,
                (float)(rect.y + rect.h) / m_atlas.height
            );
            tex->setAtlasUVs(uv);    
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
    
    void Window::addTexture(const std::shared_ptr<Texture> tex) {
        m_textures.push_back(tex);
    }
    
    void Window::SetUpdateFrameCallback(UpdateFrameCallback callback) {
        m_update_frame_callback = std::move(callback);
        if (m_update_frame_callback) {
            Debug::log("frame update callback is set");
        }
    }
    
    
    //  RENDERING
    
    void Window::draw_sprite(
        const std::shared_ptr<Sprout::Texture> texture,
        const glm::vec2 position, const glm::vec4 color_override,
        const glm::vec3 scale)
    {   
        glm::mat4 xform0 = glm::mat4(1.0f);
        
        // translate
        xform0 = glm::translate(xform0, glm::vec3(position, 0.0f));
        
        // scale
        xform0 = glm::scale(xform0, scale);
        
        glm::vec2 size = glm::vec2(texture->getWidth(), texture->getHeight());
        glm::vec4 uv = texture->getAtlasUVs();
    

        // draw
        draw_rect_projected(draw_frame.view_projection * draw_frame.camera_xform * xform0, size, uv, color_override);
    }
    
    void Window::draw_rect_projected(glm::mat4 projection, glm::vec2 size, glm::vec4 uv, glm::vec4 color_override, glm::vec4 color)
    {
        auto bottom_left = glm::vec2(0.0f, 0.0f);
        auto bottom_right = glm::vec2(size.x, 0.0f);
        auto top_left = glm::vec2(0.0f, size.y);
        auto top_right = glm::vec2(size.x, size.y);
        
        std::array<glm::vec2, 4> positions = {bottom_left, bottom_right, top_left, top_right};
        std::array<glm::vec4, 4> colors = {color, color, color, color};
        std::array<glm::vec2, 4> uvs = {
            glm::vec2(uv.x, uv.y),
            glm::vec2(uv.z, uv.y),
            glm::vec2(uv.x, uv.w),
            glm::vec2(uv.z, uv.w)
        };
        std::array<glm::vec4, 4> color_overrides = {color_override, color_override, color_override, color_override};
        
        
        draw_quad_projected(projection, positions, colors, uvs, color_overrides) ;
        
    }
    
    void Window::draw_quad_projected(
        glm::mat4 projection, 
        std::array<glm::vec2, 4> positions,
        std::array<glm::vec4, 4> colors,
        std::array<glm::vec2, 4> uvs,
        std::array<glm::vec4, 4> color_overrides)
    {
        if (draw_frame.num_quads > MAX_QUADS) {
            Debug::log("Too many quads");
            return;
        }
        
        for (int i = 0; i < 4; i++) {
            
            Vertex& vertex = draw_frame.quads[draw_frame.num_quads].vertices[i];
            
            vertex.pos = glm::vec2{(projection * glm::vec4(positions[i], 0.0f, 1.0f)).x, (projection * glm::vec4(positions[i], 0.0f, 1.0f)).y};
            vertex.color = colors[i];
            vertex.uv = uvs[i];
            vertex.color_override = color_overrides[i];
            
            // std::string debug = "pos: " + std::to_string(vertex.pos.x) + ", " + std::to_string(vertex.pos.y) + " uv: " + std::to_string(vertex.uv.x) + ", " + std::to_string(vertex.uv.y);
            // Debug::log(debug);
        }
        
        draw_frame.num_quads++;
    }

} // namespace Sprout