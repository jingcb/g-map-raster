#ifndef __TILE_MAP__H__
#define __TILE_MAP__H__
#include "map.h"
namespace gmap {
    class TileMap : Map {
    public:
        TileMap();
        ~TileMap();
        
        bool Tile(unsigned int level, unsigned int row, unsigned int col, unsigned int retina);
    private:
        unsigned int level_;
        unsigned int row_;
        unsigned int col_;
    };
}
#endif
