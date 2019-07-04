#include "map.h"

namespace gmap {
    Map::Map():
    width_(256),
    height_(256) {
    }
    
    Map::Map(int width, int height):
    width_(width),
    height_(height) {
    }
}
