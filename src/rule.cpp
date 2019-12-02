#include "rule.h"

namespace gmap {
    Rule::Rule(){}
    Rule::~Rule(){};
    
    Rule& Rule::operator=(Rule rhs) {
        using std::swap;
        std::swap(this->attributes_, rhs.attributes_);
        return *this;
    }
    
    bool Rule::HasAttribute(StyleName key) {
        if (attributes_.find(key) != attributes_.end()) {
            return true;
        } else {
            return false;
        }
    }
    
    // Call hasAttribure before this method!!!
    StyleType Rule::GetAttribute(StyleName key) {
        return attributes_.at(key);
    }
    
    void Rule::SetAttribute(StyleName key, StyleType style) {
        if (attributes_.find(key) == attributes_.end()) {
            attributes_.insert(std::make_pair(key, style));
        } else {
            attributes_[key] = style;
        }
    }
}
