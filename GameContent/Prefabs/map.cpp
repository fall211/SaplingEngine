//
//
//
//


#include "map.hpp"
#include "Debug.hpp"


namespace Prefab
{

    Map::Map(const Inst& inst)
    {
        init(inst);
    }
    
    void Map::init(const Inst& inst)
    {
        auto& map = inst->addComponent<Comp::Map>();
        map.width = M_MAP_WIDTH;
        map.height = M_MAP_HEIGHT;
        map.tileset = m_tileset;
    }

    
    void Map::SpawnTiles(const Inst& inst, const std::shared_ptr<EntityManager>& entity_manager)
    {
        auto& map = inst->getComponent<Comp::Map>();
        size_t x = 0;
        size_t y = 0;
        
        const size_t TILE_SIZE = 24;
        const size_t TILE_SCALE = 3;
        for (size_t i = 0; i < map.width * map.height; i++)
        {
            int tile = map.tileset[i] - '0';
            if (tile != 0)
            {
                auto tileEntity = entity_manager->addEntity({"static", "tile"});
                int xPos = x * TILE_SIZE * TILE_SCALE - (map.width/2 * TILE_SIZE * TILE_SCALE);
                int yPos = y * TILE_SIZE * TILE_SCALE - (map.height/2 * TILE_SIZE * TILE_SCALE);
                auto& transform = tileEntity->addComponent<Comp::Transform>(glm::vec2(xPos, yPos));
                transform.scale = glm::vec3(TILE_SCALE, TILE_SCALE, 1);
                transform.pivot = Sprout::Pivot::CENTER;
                tileEntity->addComponent<Comp::Sprite>(AssetManager::getTileSet("forest_tileset").at(tile - 1));
                tileEntity->addComponent<Comp::BBox>(24, 24).isStatic = true;
            }

            x++;
            if (x == map.width)
            {
                x = 0;
                y++;
            }
        }
    }
}

