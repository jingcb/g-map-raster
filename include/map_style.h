#ifndef __MAP_STYLE__H_
#define __MAP_STYLE__H_

#include <vector>
#include "boost/shared_ptr.hpp"
#include "rapidjson/document.h"


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
        
        inline layer_ptr GetLayer(int index) {return layers_[index];};
        inline int GetLayerCount() {return  layers_.size();};
        
    private:
        bool Parse(rapidjson::Document& document);
        bool ParseLayers(rapidjson::Document& document);
        bool ParseLayer(const rapidjson::Value &layer);
        
        std::vector<layer_ptr> layers_;
        std::string datasetDir_;
    };
    
    enum class StyleTypeName {
        INT,
        DOUBLE,
        STRING,
        VECTOR,     // {1, 2, 3}
        DOUBLEVECTOR, // {0.2, 0.3, 0.4}
        VECTOROFVECTOR // {{1, 2, 3}, {4, 5, 6}, ...}
    };
    
    class StyleName {
        
    };
}

#endif
