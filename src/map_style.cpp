#include "map_style.h"
#include "layer.h"
#include "rule.h"
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include "utils/debug_utility.h"
#include "boost/assign.hpp"
#include <boost/make_shared.hpp>

using namespace boost::assign;

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
            rapidjson::Value::ConstMemberIterator rule_itr = itr->value.FindMember("rules");
            bool has_rules = (rule_itr != itr->value.MemberEnd() && rule_itr->value.IsArray());
            if (!has_rules) {
                BOOST_LOG_TRIVIAL(error) << "Layer has no rules";
                continue;
            }
            ParseRules(rule_itr, layer);
           
            
            layers_.push_back(layer);
            
        }
        return true;
    }
    
    // ParseRules
    void MapStyle::ParseRules(const rapidjson::Value::ConstMemberIterator& rule_itr, layer_ptr layer) {
        for (rapidjson::SizeType i = 0; i < rule_itr->value.Size(); ++i) {
            rule_ptr rule = boost::make_shared<Rule>();
            for (std::map<StyleName, TypeDesc>::const_iterator type = type_desc_.begin(); type != type_desc_.end(); ++type) {
                rapidjson::Value::ConstMemberIterator attribute = rule_itr->value[i].FindMember(type->second.name_.c_str());
                if (attribute != rule_itr->value[i].MemberEnd()) {
                    std::cout<<type->second.name_<<std::endl;
                    ParseRule(type, attribute, rule);
                }
            }
    
            layer->AddRule(rule);
        }
    }
    
    // ParseStyle
    // TODO: rapidjson::array to vector 可以写成一个小函数， map类的应该也可以
    void MapStyle::ParseRule(std::map<StyleName, TypeDesc>::const_iterator type, rapidjson::Value::ConstMemberIterator attribute, rule_ptr rule) {
        switch (type->second.type_) {
            case StyleTypeName::INT: {
                if (attribute->value.IsInt()) {
                    rule->SetAttribute(type->first, attribute->value.GetInt());
                } else {

                    BOOST_LOG_TRIVIAL(warning) << type->second.name_ + " is not INT type";

                }
            }
                break;
            case StyleTypeName::DOUBLE: {
                if (attribute->value.IsDouble() || attribute->value.IsInt()) {
                    rule->SetAttribute(type->first, attribute->value.GetDouble());
                } else {

                    BOOST_LOG_TRIVIAL(warning) << type->second.name_ + " is not DOUBLE type";

                }
            }
                break;
            case StyleTypeName::STRING: {
                if (attribute->value.IsString()) {
                    std::string value = attribute->value.GetString();
                    rule->SetAttribute(type->first, value);
                } else {

                    BOOST_LOG_TRIVIAL(warning) << type->second.name_ + " is not STRING type";

                }
            }
                break;
            case StyleTypeName::VECTOR: {
                if (attribute->value.IsArray()) {
                    std::vector<int> vec;
                    bool no_error = true;
                    for (rapidjson::SizeType ii = 0; ii < attribute->value.Size(); ++ii) {
                        if (attribute->value[ii].IsInt()) {
                            vec.push_back(attribute->value[ii].GetInt());
                        } else {
                            no_error = false;

                            BOOST_LOG_TRIVIAL(warning) << type->second.name_ + " is not STD::VECTOR<INT> type";

                        }
                    }
                    if (no_error) {
                        rule->SetAttribute(type->first, vec);
                    }
                } else {

                    BOOST_LOG_TRIVIAL(warning) << type->second.name_ + " is not STD::VECTOR<INT> type";

                }
            }
                break;
            case StyleTypeName::DOUBLEVECTOR: {
                if (attribute->value.IsArray()) {
                    std::vector<double> vec;
                    bool no_error = true;
                    for (rapidjson::SizeType ii = 0; ii < attribute->value.Size(); ++ii) {
                        if (attribute->value[ii].IsDouble() || attribute->value[ii].IsInt()) {
                            vec.push_back(attribute->value[ii].GetDouble());
                        } else {
                            no_error = false;

                            BOOST_LOG_TRIVIAL(warning) << type->second.name_ + " is not STD::VECTOR<DOUBLE> type";

                        }
                    }
                    if (no_error) {
                        rule->SetAttribute(type->first, vec);
                    }
                } else {

                    BOOST_LOG_TRIVIAL(warning) << type->second.name_ + " is not STD::VECTOR<DOUBLE> type";

                }
            }
                break;
//            case StyleTypeName::VECTOROFVECTOR: {
//                if (attribute->value.IsArray() && attribute->value[rapidjson::SizeType(0)].IsArray() && attribute->value[rapidjson::SizeType(0)][rapidjson::SizeType(0)].IsInt()) {
//                    std::vector<std::vector<int> > vecofvec;
//                    for (rapidjson::SizeType ii = 0; ii < attribute->value.Size(); ++ii) {
//                        std::vector<int> vec;
//                        for (rapidjson::SizeType jj = 0; jj < attribute->value[ii].Size(); ++jj) {
//                            vec.push_back(attribute->value[ii][jj].GetInt());
//                        }
//                        vecofvec.push_back(vec);
//                    }
//                    rule->SetAttribute(type->first, vecofvec);
//                } else {
//
//                    BOOST_LOG_TRIVIAL(warning) << type->second.name_ + " is not STD::VECTOR<STD::VECTOR<INT>> type";
//
//                }
//            }
//                break;
            case StyleTypeName::MAP: {
                if (attribute->value.IsObject() && attribute->value.MemberBegin()->name.IsString() && attribute->value.MemberBegin()->value.IsInt()) {
                    std::map<std::string, int> mapData;
                    for (rapidjson::Value::ConstMemberIterator itr = attribute->value.MemberBegin(); itr != attribute->value.MemberEnd(); ++itr) {
                        mapData[itr->name.GetString()] = itr->value.GetInt();
                    }
                    rule->SetAttribute(type->first, mapData);

                } else {
                    BOOST_LOG_TRIVIAL(warning) << type->second.name_ + " is not STD::MAP<STD::STRING, INT> type";
                }
                
            }
                break;
            case StyleTypeName::MAPOFVECTOR: {
                if (attribute->value.IsObject() && attribute->value.MemberBegin()->name.IsString() && attribute->value.MemberBegin()->value.IsArray()) {
                    
                    std::map<std::string, std::vector<double>> mapData;
                    
                    for (rapidjson::Value::ConstMemberIterator itr = attribute->value.MemberBegin(); itr != attribute->value.MemberEnd(); ++itr) {
                        
                        std::vector<double> valueVec;
                        for (rapidjson::SizeType i = 0; i < itr->value.Size(); ++i) {
                                valueVec.push_back(itr->value[i].GetDouble());
                        }
                        
                        mapData[itr->name.GetString()] = valueVec;
                    }
                    rule->SetAttribute(type->first, mapData);
                    
                } else {
                    BOOST_LOG_TRIVIAL(warning) << type->second.name_ + " is not STD::MAP<STD::STRING, STD::VECTOR<DOUBLE> > type";
                }
                
            }
                break;
            case StyleTypeName::VECTOROFCOLORMAP: {
                if (attribute->value.IsArray()) {
                    std::vector<ColorMap> colorMapVec;
                    for (rapidjson::SizeType i = 0; i < attribute->value.Size(); ++i) {
                        ColorMap colorMap;
                        for (rapidjson::Value::ConstMemberIterator itr = attribute->value[i].MemberBegin(); itr != attribute->value[i].MemberEnd(); ++itr) {
                            if (itr->value.IsDouble()) {
                                colorMap.value.push_back(itr->value.GetDouble());
                            }
                            if (itr->value.IsInt()) {
                                colorMap.color.push_back(itr->value.GetInt());
                            }
                        }
                        colorMapVec.push_back(colorMap);
                    }
                    
                    rule->SetAttribute(type->first, colorMapVec);
                
                } else {
                    BOOST_LOG_TRIVIAL(warning) << type->second.name_ + " is not STD::VECTOR<COLORMAP > type";
                }
            }
                break;
            default: {
                BOOST_LOG_TRIVIAL(error) << "Not exist StyleTypeName";
                break;
            }
        }
    }
    
    std::map<StyleName, TypeDesc> MapStyle::type_desc_ = map_list_of
    (StyleName::renderType,                    TypeDesc("renderType", StyleTypeName::STRING))
    (StyleName::bands,                         TypeDesc("bands", StyleTypeName::MAP))
    (StyleName::extent,                        TypeDesc("extent", StyleTypeName::MAPOFVECTOR))
    (StyleName::nodata,                        TypeDesc("nodata", StyleTypeName::DOUBLE))
    (StyleName::band,                          TypeDesc("band", StyleTypeName::INT))
    (StyleName::colorMap,                      TypeDesc("colorMap", StyleTypeName::VECTOROFCOLORMAP));
}

