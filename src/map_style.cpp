#include "map_style.h"
#include "layer.h"
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include "utils/debug_utility.h"

namespace gmap {
    MapStyle::MapStyle() {
        
    }
    MapStyle::~MapStyle() {
        
    }
    bool MapStyle::Load(const std::string &def, bool fromFile) {
        if (fromFile) {
            return this->LoadFromFile(def);
        } else {
            return this->LoadFromStream(def.c_str());
        }
    }
    // 测试
    // 从json文件中解析风格配置
    bool MapStyle::LoadFromFile(const std::string &file) {
        std::ifstream input;
        input.open(file);
        if (!input.is_open()) {
            BOOST_LOG_TRIVIAL(error) << "can't open config file: " << file; 
        }
        
        rapidjson::IStreamWrapper isw(input);
        rapidjson::Document document;
        
        if (document.ParseStream<0, rapidjson::UTF8<>>(isw).HasParseError()) {
            input.close();
            return false;
        }
        
        if (!Parse(document)) {
            input.close();
            return false;
        } else {
            input.close();
            return true;
        }
    }
    
    // 从字节流中解析配置
    bool MapStyle::LoadFromStream(const char* jsonString) {
        rapidjson::Document document;
        
        if (document.Parse<0> (jsonString).HasParseError()) {
            return false;
        }
        
        if (!Parse(document)) {
            return false;
        } else {
            return true;
        }
        
        return true;
        
    }
    
    // 解析json字符串
    bool MapStyle::Parse(rapidjson::Document& document) {
        if (!document.IsObject()) {
            return false;
        }
        datasetDir_ = "./";
        if (document.HasMember("datasetDir")) {
            rapidjson::Value &datasetDir = document["datasetDir"];
            if (datasetDir.IsString()) {
                datasetDir_ = datasetDir.GetString();
            }
        }
        
        if (!ParseLayers(document)) {
            return false;
        }
        
        return true;
    }
    
    bool MapStyle::ParseLayer(const rapidjson::Value &layer) {
        
        
        return true;
    }

    layer_ptr MapStyle::GetLayer(int index) {
        if (index >= this->GetLayerCount()) {
            return nullptr;
        }
        return layers_[index];
    }
    
    bool MapStyle::ParseLayers(rapidjson::Document& document) {
        if (!document.HasMember("layers")) {
            BOOST_LOG_TRIVIAL(warning) << "config don't have any layers";
            return false;
        }
        rapidjson::Value &layers = document["layers"];
        if (!layers.IsObject()) {
            BOOST_LOG_TRIVIAL(error) << "Layers is not a Object";
            return false;
        }
        
        //解析每一个图层设置
        for (rapidjson::Value::ConstMemberIterator itr = layers.MemberBegin(); itr != layers.MemberEnd(); ++itr) {
            std::string layerName = itr->name.GetString();
            if (!itr->value.IsObject()) {
                BOOST_LOG_TRIVIAL(error) << "Layer: "<<layerName<<"is not a Object";
            }
            
            //获得数据
            rapidjson::Value::ConstMemberIterator  data_itr = itr->value.FindMember("data");
            std::string dataName = data_itr->value.GetString();
            layer_ptr layer = boost::make_shared<Layer>(layerName,  datasetDir_ + "/" +dataName);
            
            //获取配置
            data_itr = itr->value.FindMember("rule");
            //rapidjson::Value rule = itr->value[";
            //layer->SetRule(rule);
            
            layers_.push_back(layer);
            
        }
        return true;
    }
}

