#include "map.h"
#include "layer.h"
#include "map_style.h"
#include "COGDataSource.cpp"
#include "utils/debug_utility.h"
namespace gmap {
    Map::Map(int width, int height):
    width_(width),
    height_(height),
    xmin_(0),
    ymin_(0),
    xmax_(0),
    ymax_(0) {
        surface_ = SkSurface::MakeRasterN32Premul(width_, height_);
    }
    
    Map::~Map(){}
    void Map::SetBounds(const double &xmin, const double &ymin, const double &xmax, const double &ymax) {
        xmin_ = xmin;
        ymin_ = ymin;
        xmax_ = xmax;
        ymax_ = ymax;
    }
    
    bool Map::Init(const std::string &def, bool fromFile) {
        //初始化配置文件
        mapStyle_ = boost::make_shared<MapStyle>();
        return mapStyle_->Load(def, fromFile);
    }
    
    bool Map::Render() {
        int nlayers = mapStyle_->GetLayerCount();
        cogDataSource = boost::make_shared<COGDataSource>();
        for(int i = 0; i < nlayers; i++) {
            layer_ptr layer = mapStyle_->GetLayer(i);
            if (layer == nullptr) {
                return false;
            }
            
            cogDataSource->SetDataPath(layer->GetDataPath());
            cogDataSource->SetSpitialFilter(xmin(), ymin(), xmax(), ymax());
            float* imageData = new float[width_ * height_];
            if(!cogDataSource->ReadRaster(width_, height_, imageData)) {
                delete []imageData;
                return false;
            }
            
            
            delete []imageData;
        }
        return true;
    }
}
