#include "map_style.h"
namespace gmap {
    bool MapStyle::Load(const std::string &def, bool fromFile) {
        if (fromFile) {
            return this->LoadFromFile(def);
        } else {
            return this->LoadFromStream(def.c_str());
        }
    }
    bool MapStyle::LoadFromStream(const char* jsonString) {
        return true;
    }
}

