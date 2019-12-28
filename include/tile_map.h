#ifndef __TILE_MAP__H__
#define __TILE_MAP__H__
#include "map.h"
#include <vector>
namespace gmap {
    const int MAP_LEVEL_COUNT = 20;
    constexpr double ZOOM_RESES[MAP_LEVEL_COUNT] = {
        156543.033928,
        78271.516964,
        39135.758482,
        19567.879241,
        9783.9396205,
        4891.96981025,
        2445.984905125,
        1222.9924525625,
        611.49622628125,
        305.748113140625,
        152.8740565703125,
        76.43702828515625,
        38.21851414257813,
        19.10925707128906,
        9.55462853564453,
        4.777314267822266,
        2.388657133911133,
        1.194328566955567,
        0.597164283477783,
        0.298582141738892
    };
    class TileMap : public Map {
    public:
        TileMap(unsigned int level, unsigned int row, unsigned int col);
        ~TileMap();
        bool Tile(const std::string& style, const bool& fromfile, const unsigned int retina = 1);
        bool CalculateTileExtent();
        
    private:
        unsigned int level_;
        unsigned int row_;
        unsigned int col_;
        
        
    private:
        static constexpr double originX_ = -20037508.342787;
        static constexpr double originY_ = 20037508.342787;
        
        
        static constexpr int tileSize_ = 256;
    };
    
    
}
#endif
