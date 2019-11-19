//
//  COGDataSource.cpp
//  products
//
//  Created by chenbo jing on 2019/11/18.
//

#include "COGDataSource.h"
namespace gmap {
    COGDataSource::COGDataSource() : filepath_("") {
        
    }
    COGDataSource::~COGDataSource() {
        
    }
    void COGDataSource::SetSpitialFilter(double xmin, double ymin, double xmax, double ymax) {
        xmin_ = xmin;
        ymin_ = ymin;
        xmax_ = xmax;
        ymax_ = ymax;
    }
    bool COGDataSource::RasterIO(const int& width, const int& height) {
        return false;
    }
    bool COGDataSource::ReadRaster(const int& width, const int& height, void* imageData) {
        return false;
    }
    
}
