#include "map.h"
#include "layer.h"
#include "map_style.h"
#include "RenderBase.h"
#include "COGDataSource.h"
#include "utils/debug_utility.h"

#include "include/core/SkImageEncoder.h"

#include "include/core/SkImage.h"
#include "include/core/SkEncodedImageFormat.h"

namespace gmap {
    Map::Map() {};
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
       
        for(int i = 0; i < nlayers; i++) {
            layer_ptr layer = mapStyle_->GetLayer(i);
            if (layer == nullptr) {
                return false;
            }
            cogdatasource_ptr cogDataSource = layer->GetDataSource(xmin(), ymin(), xmax(), ymax());
//            float* imageData = new float[width_ * height_];
//            if(!cogDataSource->ReadRaster(width_, height_, imageData)) {
//                delete []imageData;
//                return false;
//            }
            for (int j = 0; j < layer->GetRuleCount(); ++j) {
                rule_ptr rule = layer->GetRule(i);
                RenderBase render(surface_);
                render.Render(rule, cogDataSource);
            }
            
            
        }
        return true;
    }
    
    bool Map::SaveFile(const std::string filePath) {
        sk_sp<SkImage> img(surface_->makeImageSnapshot());
        
        sk_sp<SkData> png(img->encodeToData(SkEncodedImageFormat::kPNG, 100));
        SkFILEWStream out(filePath.c_str());
        if (!img) {
            return false;
        }
        
        (void)out.write(png->data(), png->size());
        return true;
    }
    bool Map::SaveStream(std::string &stream) {
        SkDynamicMemoryWStream wStream;
        SkImageInfo imageInfo;
        
        imageInfo = SkImageInfo::MakeN32Premul(width_, height_);
        bitmap_.allocPixels(imageInfo);
        surface_->readPixels(bitmap_, 0, 0);
        if(!SkEncodeImage(&wStream, bitmap_, SkEncodedImageFormat::kPNG, 100)) {
            return false;
        }
        int length = wStream.bytesWritten();
        char* imageChar = new char[length];
        wStream.copyTo(imageChar);
        wStream.reset();
        
        stream = std::string(imageChar, length);
        
        delete[] imageChar;
        return true;
    }
}
