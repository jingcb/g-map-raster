#include "layer.h"
#include "COGDataSource.h"
#include <boost/make_shared.hpp>

namespace gmap {
    Layer::Layer():
    layerName_(""),
    dataPath_("") {
        
    };
    
    Layer::Layer(std::string layerName, std::string dataPath):
    layerName_(layerName),
    dataPath_(dataPath) {
        
    };
    rule_ptr Layer::GetRule(const int& idx) {
        if (idx < ruleVec_.size()) {
            return ruleVec_[idx];
        }
        return nullptr;
    }
    cogdatasource_ptr Layer::GetDataSource(const double& xmin, const double& ymin, const double& xmax, const double& ymax){
        cogdatasource_ptr cogDataSource = boost::make_shared<COGDataSource>();
        
        cogDataSource->SetSpitialFilter(xmin, ymin, xmax, ymax);
        return cogDataSource;
    }
}
