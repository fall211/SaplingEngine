//
//
//
//

#pragma once

#include <cstddef>
#include <utility>

#include "Component.hpp"
#include "Entity.hpp"
#include "Debug.hpp"

namespace Comp
{
    class MapData : public Component 
    {
        public:
            glm::vec2 position;
            int TILE_SIZE;
            std::vector<std::vector<std::string>> mapData;
            explicit MapData(Inst inst, std::vector<std::vector<std::string>> data, glm::vec2 pos, int tileSize = 24) 
            :   Component(std::move(inst)), 
                position(pos),
                TILE_SIZE(tileSize),
                mapData(std::move(data))
            {}
            
            int getTile(int worldX, int worldY) 
            {
                // change x and y to tile coordinates
                size_t x = (worldX - position.x + TILE_SIZE/2) / TILE_SIZE;
                size_t y = (worldY - position.y + TILE_SIZE/2) / TILE_SIZE;
                

                if (x < 0 || y < 0 || y >= mapData.size() || x >= mapData[y].size())
                {
                    return -1;
                }
                
                return std::stoi(mapData[y][x]);
            }
    };
}