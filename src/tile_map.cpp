#include "tile_map.h"
#include "utils/debug_utility.h"

namespace gmap {
    TileMap::TileMap(unsigned int level, unsigned int row, unsigned int col) : level_(level), row_(row), col_(col) {
    };
    TileMap::~TileMap() {
    }
    bool TileMap::Tile(const std::string& style, const bool& fromfile, const unsigned int retina) {
        
        BOOST_LOG_TRIVIAL(info) << "(level, row, col) = (" << level_ << ", " << row_ << ", " << col_ << ")";
        
        this->width_ = this->tileSize_ * retina;
        this->height_ = this->tileSize_ * retina;
        if (surface_ == nullptr) {
            surface_ = SkSurface::MakeRasterN32Premul(width_, height_);
        }
        if (!Map::Init(style, fromfile)) {
            BOOST_LOG_TRIVIAL(error) << "Load style fail !!!";
            return false;
        }
        if (!CalculateTileExtent()) {
            BOOST_LOG_TRIVIAL(error) << "Calculate tile fail!";
            return false;
        }
        
        if (!Map::Render()) {
            BOOST_LOG_TRIVIAL(error) << "Render tile fail!";
            return false;
        }
        return true;
    }
    bool TileMap::CalculateTileExtent() {
        BOOST_LOG_TRIVIAL(info) << "calculate tile";
        if (level_ < 0 || row_ < 0 || col_ < 0) {
            BOOST_LOG_TRIVIAL(warning) << "Tile index illegal(z, x, y): " << level_ << ", " << row_ << ", " << col_;
            return false;
        }
        
        double tileExtent = this->tileSize_ * ZOOM_RESES[level_];
        double tileOriginalX = this->originX_ + row_ * tileExtent;
        double tileOriginalY = this->originY_ - col_ * tileExtent;
        xmin_ = tileOriginalX;
        ymin_ = tileOriginalY - tileExtent;
        xmax_ = tileOriginalX + tileExtent;
        ymax_ = tileOriginalY;
        return true;
    }
}
