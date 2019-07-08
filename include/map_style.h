#ifndef __MAP_STYLE__H_
#define __MAP_STYLE__H_

#include <vector>
#include "boost/shared_ptr.hpp"


namespace gmap {
    class Layer;
    using layer_ptr = boost::shared_ptr<Layer>;
    class MapStyle {
    public:
        MapStyle();
        ~MapStyle();
        
        bool Load(const std::string &def, bool fromFile);
        bool LoadFromFile(const std::string &file);
        bool LoadFromStream(const char* jsonString);
    };
}

#endif
