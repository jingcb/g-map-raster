#ifndef __MAP_STYLE__H_
#define __MAP_STYLE__H_

#include <vector>
#include "rule.h"
#include "boost/shared_ptr.hpp"
#include "rapidjson/document.h"


namespace gmap {
    class Layer;
    using layer_ptr = boost::shared_ptr<Layer>;
    using rule_ptr = boost::shared_ptr<Rule>;
    class MapStyle {
    public:
        MapStyle();
        ~MapStyle();
        
        bool Load(const std::string &def, bool fromFile);
        bool LoadFromFile(const std::string &file);
        bool LoadFromStream(const char* jsonString);
        
        layer_ptr GetLayer(int index);
        inline int GetLayerCount() {return  layers_.size();};
        
    private:
        bool Parse(rapidjson::Document& document);
        bool ParseLayers(rapidjson::Document& document);
        void ParseRules(const rapidjson::Value::ConstMemberIterator& rule_itr, layer_ptr layer);
        void ParseStyle(std::map<StyleName, TypeDesc>::const_iterator type, rapidjson::Value::ConstMemberIterator attribute, rule_ptr rule);
        std::vector<layer_ptr> layers_;
        std::string datasetDir_;
        static std::map<StyleName, TypeDesc> type_desc_;
    };
    
}

#endif
