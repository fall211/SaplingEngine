//
//  Window.cpp
//  SaplingEngine, Sprout Renderer
//

//  This file contains all the window functionality that is 
//  related to windowing but not part of sokol implementation.
//  For the sokol window setup, see Sprout.mm

#include "Sprout.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cmath>
#include <memory>

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb/stb_rect_pack.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

namespace Sprout
{

    void Window::bake_atlas()
    {
        const int PADDING = 2;
            m_atlas.width = 512;
            m_atlas.height = 512;
            
            unsigned char* atlas_data = (unsigned char*)malloc(m_atlas.width * m_atlas.height * 4);
            memset((void*)atlas_data, 0, m_atlas.width * m_atlas.height * 4);
            
            stbrp_context context;
            stbrp_node* nodes = (stbrp_node*)malloc(sizeof(stbrp_node) * m_atlas.width);
            stbrp_init_target(&context, m_atlas.width, m_atlas.height, nodes, m_atlas.width);
            
            for (auto& tex : m_textures) {
                stbrp_rect rect;
                rect.id = 0;
                rect.w = tex->getWidth() + (PADDING * 2);
                rect.h = tex->getHeight() + (PADDING * 2);
                
                if (!stbrp_pack_rects(&context, &rect, 1)) {
                    throw std::runtime_error("Failed to pack rects");
                    continue;
                }
                
                // copy texture data
                for (int y = 0; y < tex->getHeight(); y++) {
                    for (int x = 0; x < tex->getWidth(); x++) {
                        int src_index = (y * tex->getWidth() + x) * 4;
                        int dst_index = ((rect.y + PADDING + y) * m_atlas.width + (rect.x + PADDING + x)) * 4;
                        
                        atlas_data[dst_index + 0] = tex->getPixels()[src_index + 0];
                        atlas_data[dst_index + 1] = tex->getPixels()[src_index + 1];
                        atlas_data[dst_index + 2] = tex->getPixels()[src_index + 2];
                        atlas_data[dst_index + 3] = tex->getPixels()[src_index + 3];
                    }
                }
                
                // extend edges into padding
                int base_x = rect.x + PADDING;
                int base_y = rect.y + PADDING;
                int width = tex->getWidth();
                int height = tex->getHeight();
                
                // horizontal edges
                for (int x = 0; x < width; x++) {
                    // T
                    for (int p = 1; p <= PADDING; p++) {
                        int src_index = ((base_y) * m_atlas.width + (base_x + x)) * 4;
                        int dst_index = ((base_y - p) * m_atlas.width + (base_x + x)) * 4;
                        memcpy(&atlas_data[dst_index], &atlas_data[src_index], 4);
                    }
                    
                    // B
                    for (int p = 1; p <= PADDING; p++) {
                        int src_index = ((base_y + height - 1) * m_atlas.width + (base_x + x)) * 4;
                        int dst_index = ((base_y + height - 1 + p) * m_atlas.width + (base_x + x)) * 4;
                        memcpy(&atlas_data[dst_index], &atlas_data[src_index], 4);
                    }
                }
                
                // vertical edges
                for (int y = 0; y < height; y++) {
                    // L
                    for (int p = 1; p <= PADDING; p++) {
                        int src_index = ((base_y + y) * m_atlas.width + base_x) * 4;
                        int dst_index = ((base_y + y) * m_atlas.width + (base_x - p)) * 4;
                        memcpy(&atlas_data[dst_index], &atlas_data[src_index], 4);
                    }
                    
                    // R
                    for (int p = 1; p <= PADDING; p++) {
                        int src_index = ((base_y + y) * m_atlas.width + (base_x + width - 1)) * 4;
                        int dst_index = ((base_y + y) * m_atlas.width + (base_x + width - 1 + p)) * 4;
                        memcpy(&atlas_data[dst_index], &atlas_data[src_index], 4);
                    }
                }
                
                // corners
                for (int py = 1; py <= PADDING; py++) {
                    for (int px = 1; px <= PADDING; px++) {
                        // TL
                        memcpy(
                            &atlas_data[((base_y - py) * m_atlas.width + (base_x - px)) * 4],
                            &atlas_data[(base_y * m_atlas.width + base_x) * 4],
                            4
                        );
                        
                        // TR
                        memcpy(
                            &atlas_data[((base_y - py) * m_atlas.width + (base_x + width - 1 + px)) * 4],
                            &atlas_data[(base_y * m_atlas.width + (base_x + width - 1)) * 4],
                            4
                        );
                        
                        // BL
                        memcpy(
                            &atlas_data[((base_y + height - 1 + py) * m_atlas.width + (base_x - px)) * 4],
                            &atlas_data[((base_y + height - 1) * m_atlas.width + base_x) * 4],
                            4
                        );
                        
                        // BR
                        memcpy(
                            &atlas_data[((base_y + height - 1 + py) * m_atlas.width + (base_x + width - 1 + px)) * 4],
                            &atlas_data[((base_y + height - 1) * m_atlas.width + (base_x + width - 1)) * 4],
                            4
                        );
                    }
                }
                
                glm::vec4 uv = glm::vec4(
                    (float)(rect.x + PADDING) / m_atlas.width,
                    (float)(rect.y + PADDING) / m_atlas.height,
                    (float)(rect.x + PADDING + tex->getWidth()) / m_atlas.width,
                    (float)(rect.y + PADDING + tex->getHeight()) / m_atlas.height
                );
                tex->setAtlasUVs(uv);
            }
            
            sg_image_desc img_desc = {};
            img_desc.width = m_atlas.width;
            img_desc.height = m_atlas.height;
            img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
            img_desc.data.subimage[0][0].ptr = atlas_data;
            img_desc.data.subimage[0][0].size = (size_t)m_atlas.width * m_atlas.height * 4;
 
            
            m_atlas.img = sg_make_image(&img_desc);
            
            stbi_write_png("atlas.png", m_atlas.width, m_atlas.height, 4, atlas_data, m_atlas.width * 4);
            
            free(atlas_data);
            free(nodes);

    }
    
    void Window::addTexture(const std::shared_ptr<Texture> tex)
    {
        m_textures.push_back(tex);
    }
    
    void Window::SetUpdateFrameCallback(UpdateFrameCallback callback)
    {
        m_update_frame_callback = std::move(callback);
        if (m_update_frame_callback) {
        }
    }
    
    void Window::SetEventCallback(EventCallback callback)
    {
        m_event_callback = std::move(callback);
    }
    
    
    //  RENDERING
    
    void Window::draw_sprite(
        const std::shared_ptr<Sprout::Texture> texture,
        const glm::vec2 position, 
        const glm::f32 layer,
        const glm::f32 rotation,
        const glm::i32 frameNum,
        const glm::vec4 color_override,
        const glm::vec3 scale,
        const Pivot pivot)
    {   
        glm::mat4 xform0 = glm::mat4(1.0f);
        
        // translate
        xform0 = glm::translate(xform0, glm::vec3(position, 0.0f));
        
        // scale
        xform0 = glm::scale(xform0, scale);
        
        // rotate
        xform0 = glm::rotate(xform0, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        
        glm::vec2 size = texture->getSize();                
        glm::vec2 frame_size = texture->getFrameSize();
        glm::vec4 uv = texture->getAtlasUVs();
        
        if (frame_size != size)
        {
            glm::i32 numFrames = texture->getNumFrames();
            
            glm::vec4 offset_uv = glm::vec4(
                uv.x + ((uv.z - uv.x) / (float)numFrames) * (float)frameNum,
                uv.y,
                uv.x + ((uv.z - uv.x) / (float)numFrames) * ((float)frameNum + 1),
                uv.w
            );
            uv = offset_uv;
        }
        
        // draw
        draw_rect_projected(draw_frame.view_projection * draw_frame.camera_xform * xform0, frame_size, layer, uv, color_override, pivot);
    }
    
    void Window::draw_rectangle(float x, float y, float width, float height, const std::shared_ptr<Sprout::Texture> texture, glm::vec4 color)
    {
        // draw rectangle
        auto xform = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
        glm::vec4 uv = texture->getAtlasUVs();

        draw_rect_projected(draw_frame.view_projection * draw_frame.camera_xform * xform, glm::vec2(width, height), 1.0f, uv, color, Pivot::CENTER);
    }
    
    void Window::draw_rect_projected(
        glm::mat4 projection,
        glm::vec2 size,
        glm::f32 layer,
        glm::vec4 uv,
        glm::vec4 color_override,
        Pivot pivot)
    {
        
        glm::vec2 pivot_offset = getPivotOffset(pivot) * size;

        glm::vec4 bottom_left = glm::vec4(0.0f - pivot_offset.x, 0.0f - pivot_offset.y, layer, 1.0f);
        glm::vec4 bottom_right = glm::vec4(size.x - pivot_offset.x, 0.0f - pivot_offset.y, layer, 1.0f);
        glm::vec4 top_left = glm::vec4(0.0f - pivot_offset.x, size.y - pivot_offset.y, layer, 1.0f);
        glm::vec4 top_right = glm::vec4(size.x - pivot_offset.x, size.y - pivot_offset.y, layer, 1.0f);
                
        
        std::array<glm::vec4, 4> positions = {bottom_left, top_left, top_right, bottom_right};
        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        std::array<glm::vec4, 4> colors = {color, color, color, color};
        std::array<glm::vec2, 4> uvs = {
            glm::vec2(uv.x, uv.y),
            glm::vec2(uv.x, uv.w),
            glm::vec2(uv.z, uv.w),
            glm::vec2(uv.z, uv.y)
        };
        std::array<glm::vec4, 4> color_overrides = {color_override, color_override, color_override, color_override};
        
        draw_quad_projected(projection, positions, colors, uvs, color_overrides) ; 
    }
    
    void Window::draw_quad_projected(
        glm::mat4 projection, 
        std::array<glm::vec4, 4> positions,
        std::array<glm::vec4, 4> colors,
        std::array<glm::vec2, 4> uvs,
        std::array<glm::vec4, 4> color_overrides)
    {
        if (draw_frame.num_quads > MAX_QUADS)
        {
            //throw std::runtime_error("Too many quads");
            return;
        }
        
        for (int i = 0; i < 4; i++)
        {
            Vertex& vertex = draw_frame.quads[draw_frame.num_quads].vertices[i];
            
            vertex.pos = projection * positions[i];
            vertex.color = colors[i];
            vertex.uv = uvs[i];
            vertex.color_override = color_overrides[i];
        }
        
        draw_frame.num_quads++;
    }
    
    glm::vec2 Window::screenToWorld(glm::vec2 screen_pos) 
    {
        glm::mat4 inv_proj = glm::inverse(Window::instance->draw_frame.view_projection * Window::instance->draw_frame.camera_xform);
    
        // normalize screen pos
        float normal_x = (screen_pos.x) / Window::instance->getWidth() - 1.0f;
        float normal_y = 1.0f - (screen_pos.y) / Window::instance->getHeight();
        glm::vec4 ndc_pos = glm::vec4(normal_x, normal_y, 0.0f, 1.0f);
    
        glm::vec4 world_pos = inv_proj * ndc_pos;
        return glm::vec2(world_pos.x, world_pos.y);
    }

    void Window::translateCamera(glm::f32 deltaX, glm::f32 deltaY)
    {
        draw_frame.camera_xform = glm::translate(
            draw_frame.camera_xform, 
            glm::vec3(-deltaX, -deltaY, 0.0f)
        );
    }
    
    void Window::setCameraPosition(glm::vec2 position)
    {
        glm::vec3 scale = glm::vec3(
            draw_frame.camera_xform[0][0],
            draw_frame.camera_xform[1][1],
            draw_frame.camera_xform[2][2]
        );
        glm::mat4 xform = glm::mat4(1.0f);
        xform = glm::scale(xform, scale);
        xform = glm::translate(xform, glm::vec3(-position, 0.0f));
        draw_frame.camera_xform = xform;
    }
    
    glm::vec2 Window::getCameraPosition()
    {
        glm::vec2 scale = glm::vec2(
            draw_frame.camera_xform[0][0],
            draw_frame.camera_xform[1][1]
        );
        glm::vec2 position = glm::vec2(
            -draw_frame.camera_xform[3][0],
            -draw_frame.camera_xform[3][1]
        );
        return position / scale;
    }
    
}