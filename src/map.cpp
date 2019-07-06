#include "map.h"
#include "map_style.h"
namespace gmap {
    Map::Map():
    width_(256),
    height_(256) {
    }
    
    Map::Map(int width, int height):
    width_(width),
    height_(height) {
    }
    
    bool Map::Init(const std::string &def) {
        //初始化配置文件
        return mapStyle_->Load(def);
    }
    
}
