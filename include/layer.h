#ifndef __LAYER__H__
#define __LAYER__H__

#include "rapidjson/document.h"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

namespace gmap {
    //存储图层信息，包括图层用到的数据，渲染图层的风格
    
    class Rule;
    using rule_ptr = boost::shared_ptr<Rule>;
    class COGDataSource;
    using cogdatasource_ptr = boost::shared_ptr<COGDataSource>;
    class Layer {
    public:
        Layer();
        Layer(std::string layerName, std::string dataPath);
        
        //设置图层渲染的风格
        void AddRule(rule_ptr rule) { if(rule != nullptr) ruleVec_.push_back(rule); };
        rule_ptr GetRule(const int& idx);
        int GetRuleCount() { return ruleVec_.size(); };
        std::string GetDataPath() { return dataPath_;};
        cogdatasource_ptr GetDataSource(const double& xmin, const double& ymin, const double& xmax, const double& ymax);
    private:
        std::string layerName_;
        std::string dataPath_;
        std::vector<rule_ptr> ruleVec_;
        
    };
    
}
#endif 
