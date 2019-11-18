#include "layer.h"

namespace gmap {
    Layer::Layer():
    layerName_(""),
    dataPath_("") {};
    
    Layer::Layer(std::string layerName, std::string dataPath):
    layerName_(layerName),
    dataPath_(dataPath) {};
}
