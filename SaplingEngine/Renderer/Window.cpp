//
//  Window.cpp
//  SaplingEngine, Sprout Renderer
//

//  This file contains all the window functionality that is 
//  related to windowing but not part of sokol implementation.
//  For the sokol window setup, see Sprout.mm

#include "Debug.hpp"
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
        
        for (auto& tex : m_textures)
        {
            stbrp_rect rect;
            rect.id = 0;
            rect.w = tex->getWidth();
            rect.h = tex->getHeight();
            
            if (!stbrp_pack_rects(&context, &rect, 1))
            {
                Debug::log("Failed to pack rect");
                continue;
            }
            
            // copy data to atlas
            for (int y = 0; y < tex->getHeight(); y++)
            {
                for (int x = 0; x < tex->getWidth(); x++)
                {
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
    
    void Window::addTexture(const std::shared_ptr<Texture> tex)
    {
        m_textures.push_back(tex);
    }
    
    void Window::SetUpdateFrameCallback(UpdateFrameCallback callback)
    {
        m_update_frame_callback = std::move(callback);
        if (m_update_frame_callback) {
            Debug::log("frame update callback is set");
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
            Debug::log("Too many quads");
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
        glm::mat4 xform = glm::translate(glm::mat4(1.0f), glm::vec3(deltaX, deltaY, 0.0f));
        draw_frame.camera_xform = xform * draw_frame.camera_xform;
    }
    
    void Window::setCameraPosition(glm::vec2 position)
    {
        glm::mat4 xform = glm::translate(glm::mat4(1.0f), glm::vec3(-position, 0.0f));
        draw_frame.camera_xform = xform;
    }
    
}