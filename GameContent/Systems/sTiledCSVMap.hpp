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
#include "cEnemySpawner.hpp"
#include "cMapData.hpp"
#include "Sprout.hpp"
#include <cstddef>
#include <fstream>
#include <memory>
#include <sstream>
#include <enemy.hpp>

namespace System
{   
    
    inline std::shared_ptr<Entity> SpawnCollisionOptimizedMap(const std::string& mapFile, EntityManager& entManager) {
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

        
        const int TILE_SIZE = 24;
        
        auto mapEntity = entManager.addEntity({"map"});
        auto& mapTransform = mapEntity->addComponent<Comp::Transform>(glm::vec2(-240,-1440));
        auto& mapDataComponent = mapEntity->addComponent<Comp::MapData>(mapData, mapTransform.position, TILE_SIZE);
        mapEntity->addComponent<Comp::TransformHierarchy>();
    
        std::vector<std::vector<bool>> visited(mapData.size(), 
            std::vector<bool>(mapData[0].size(), false));
    
        auto isSolidTile = [&](size_t y, size_t x, bool includeObjects = false) -> bool {
            if (y < 0 || y >= mapData.size() || x < 0 || x >= mapData[y].size()) 
                return false;
            
            if (includeObjects)
                return std::stoi(mapData[y][x]) != -1;
            else 
                return std::stoi(mapData[y][x]) != -1  && std::stoi(mapData[y][x]) <= mapDataComponent.NUM_INDIVIDUAL_TILES;
        };
    
        auto findLargestRectangle = [&](size_t startY, size_t startX) {
            size_t hWidth = 1, hHeight = 1;
            while (startX + hWidth < mapData[startY].size() && 
                   isSolidTile(startY, startX + hWidth)) {
                hWidth++;
            }

            bool canExpandVertically = true;
            while (startY + hHeight < mapData.size() && canExpandVertically) {
                for (size_t x = startX; x < startX + hWidth; x++) {
                    if (!isSolidTile(startY + hHeight, x)) {
                        canExpandVertically = false;
                        break;
                    }
                }
                if (canExpandVertically) hHeight++;
            }
            
            size_t vWidth = 1, vHeight = 1;
            
            while (startY + vHeight < mapData.size() && 
                   isSolidTile(startY + vHeight, startX)) {
                vHeight++;
            }

            bool canExpandHorizontally = true;
            while (startX + vWidth < mapData[startY].size() && canExpandHorizontally) {
                for (size_t y = startY; y < startY + vHeight; y++) {
                    if (!isSolidTile(y, startX + vWidth)) {
                        canExpandHorizontally = false;
                        break;
                    }
                }
                if (canExpandHorizontally) vWidth++;
            }
            
            if (hWidth * hHeight >= vWidth * vHeight) {
                return std::make_pair(hWidth, hHeight);
            } else {
                return std::make_pair(vWidth, vHeight);
            }
        };
    
        for (size_t y = 0; y < mapData.size(); y++) {
            for (size_t x = 0; x < mapData[y].size(); x++) {
                if (visited[y][x] || !isSolidTile(y, x)) continue;
    
                auto [width, height] = findLargestRectangle(y, x);
    
                for (size_t dy = 0; dy < height; dy++) {
                    for (size_t dx = 0; dx < width; dx++) {
                        visited[y + dy][x + dx] = true;
                    }
                }
    
                auto tileEntity = entManager.addEntity({"static", "tile", "physicsenabled"});
                
                float xPos = (x + width/2.0f) * TILE_SIZE + mapTransform.position.x - TILE_SIZE / 2.0f;
                float yPos = (y + height/2.0f) * TILE_SIZE + mapTransform.position.y - TILE_SIZE / 2.0f;
                
                auto& transform = tileEntity->addComponent<Comp::Transform>(glm::vec2(xPos, yPos));
                transform.pivot = Sprout::Pivot::CENTER;
                
                tileEntity->addComponent<Comp::BBox>(width * TILE_SIZE, height * TILE_SIZE);
                tileEntity->addComponent<Comp::TransformHierarchy>().setParent(mapEntity);
                entManager.getSpatialGrid().updateEntity(tileEntity);
            }
        }
        
        // add visual sprites
        for (size_t y = 0; y < mapData.size(); y++) {
            for (size_t x = 0; x < mapData[y].size(); x++) {
                if (!isSolidTile(y, x, true)) continue;

                int tile = std::stoi(mapData[y][x]);
                auto spriteEntity = entManager.addEntity({"map_sprite"});
                float spriteX = x * TILE_SIZE + mapTransform.position.x;
                float spriteY = y * TILE_SIZE + mapTransform.position.y;
                auto& spriteTransform = spriteEntity->addComponent<Comp::Transform>(glm::vec2(spriteX, spriteY));
                spriteTransform.pivot = Sprout::Pivot::CENTER;

                switch (tile)
                {
                    case 97: // tree
                        spriteEntity->addComponent<Comp::Sprite>(AssetManager::getTexture("tree"));
                        spriteTransform.pivot = Sprout::Pivot::BOTTOM_CENTER;
                        spriteTransform.position.y += TILE_SIZE / 2 + 2;
                        break;
                    case 98: // fern
                        spriteEntity->addComponent<Comp::Sprite>(AssetManager::getTexture("fern"), 6);
                        spriteEntity->addComponent<Comp::EnemySpawner<Prefab::ForestSpirit>>();
                        break;

                    default:
                        spriteEntity->addComponent<Comp::Sprite>(AssetManager::getTileSet("game_tileset").at(tile));
                        break;
                }

                entManager.getSpatialGrid().updateEntity(spriteEntity);
            }
        }
        
        return mapEntity;
    }
}