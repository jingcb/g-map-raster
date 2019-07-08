#include "map.h"
#include "map_style.h"
#include <boost/make_shared.hpp>
#include "utils/debug_utility.h"
namespace gmap {
    Map::Map():
    width_(256),
    height_(256) {
    }
    
    Map::Map(int width, int height):
    width_(width),
    height_(height) {
    }
    
    void Map::SetBounds(const double &xmin, const double &ymin, const double &xmax, const double &ymax) {
        xmin_ = xmin;
        ymin_ = ymin;
        xmax_ = xmax;
        ymax_ = ymax;
    }
    
    bool Map::Init(const std::string &def, bool fromFile) {
        //初始化配置文件
        mapStyle_ = boost::make_shared<MapStyle>();
        return mapStyle_->Load(def, fromFile);
    }
    
}
