#ifndef __MAP_STYLE__H_
#define __MAP_STYLE__H_

namespace gmap {
    class MapStyle {
    public:
        MapStyle();
        ~MapStyle();
        bool Load(const std::string &def);
    };
}

#endif
