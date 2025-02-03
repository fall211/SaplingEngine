//
//
//
//

#pragma once

#include "Debug.hpp"
#include "EntityManager.hpp"
#include "Seedbank/AssetManager.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "Sprout.hpp"
#include <MacTypes.h>
#include <cstddef>
#include <fstream>
#include <memory>
#include <sstream>

namespace System
{
    inline std::shared_ptr<Entity> SpawnMap(const std::string& mapFile , EntityManager& entManager)
    {
        // read in the csv file
        std::ifstream file(mapFile);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + mapFile);
        }
        
        std::string line;
        std::vector<std::vector<std::string>> mapData;
        
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string cell;
            std::vector<std::string> row;
    
            while (std::getline(ss, cell, ',')) {
                row.push_back(cell);
            }
            mapData.push_back(row);        
        }
        
        file.close();
        
        auto mapEntity = entManager.addEntity({"map"});
        auto& mapTransform = mapEntity->addComponent<Comp::Transform>(glm::vec2(-240,0));
        mapEntity->addComponent<Comp::TransformHierarchy>();
        
        int TILE_SIZE = 24;
        
        for (size_t y = 0; y < mapData.size(); y++)
        {
            for (size_t x = 0; x < mapData[y].size(); x++)
            {
                int tile = std::stoi(mapData[y][x]);
                if (tile != -1 && tile != 13)
                {
                    auto tileEntity = entManager.addEntity({"static", "tile"});
                    
                    int xPos = x * TILE_SIZE + mapTransform.position.x;
                    int yPos = y * TILE_SIZE + mapTransform.position.y;
                    auto& transform = tileEntity->addComponent<Comp::Transform>(glm::vec2(xPos, yPos));
                    transform.pivot = Sprout::Pivot::TOP_CENTER;
                    tileEntity->addComponent<Comp::Sprite>(AssetManager::getTileSet("forest_tileset").at(tile));
                    tileEntity->addComponent<Comp::BBox>(24, 24);
                    tileEntity->addComponent<Comp::TransformHierarchy>().setParent(mapEntity);
                }
                else if (tile == 13)
                {
                    Debug::log("Spawned fern");
                    auto tileEntity = entManager.addEntity({"fern"});
                    
                    int xPos = x * TILE_SIZE + mapTransform.position.x;
                    int yPos = y * TILE_SIZE + mapTransform.position.y;
                    auto& transform = tileEntity->addComponent<Comp::Transform>(glm::vec2(xPos, yPos));
                    transform.pivot = Sprout::Pivot::TOP_CENTER;
                    tileEntity->addComponent<Comp::Sprite>(AssetManager::getTexture("fern"), 5);
                    tileEntity->addComponent<Comp::TransformHierarchy>().setParent(mapEntity);
                }
            }
        }
        
        return mapEntity;
    }
}