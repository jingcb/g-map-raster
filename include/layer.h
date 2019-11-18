#ifndef __LAYER__H__
#define __LAYER__H__

#include "rapidjson/document.h"
#include <boost/shared_ptr.hpp>

namespace gmap {
    //存储图层信息，包括图层用到的数据，渲染图层的风格
    
    class Layer {
    public:
        Layer();
        Layer(std::string layerName, std::string dataPath);
        
        //设置图层渲染的风格
        void SetRule(rapidjson::Value rule){rule_ = rule;};
        std::string GetDataPath() { return dataPath_;};
    private:
        std::string layerName_;
        std::string dataPath_;
        rapidjson::Value rule_;
    };
}
#endif 
