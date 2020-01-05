#include "RenderBase.h"

#include "rule.h"
#include "utils/debug_utility.h"
#include "COGDataSource.h"
#include "include/core/SkColor.h"
#include "include/core/SkCanvas.h"


namespace gmap {
    RenderBase::RenderBase(sk_sp<SkSurface> surface) : surface_(surface) {
        
    };
    RenderBase::~RenderBase() {
        
    };
    bool RenderBase::Render(rule_ptr rule, cogdatasource_ptr dataSource) {
        if(!rule->HasAttribute(StyleName::renderType)) {
            BOOST_LOG_TRIVIAL(error) << "rule don't have attribute: renderType";
            return false;
        }
        std::string renderType = boost::get<std::string>(rule->GetAttribute(StyleName::renderType));
        BOOST_LOG_TRIVIAL(info) << "****** --- renderType: " << renderType << " --- ******";
        if (renderType == "category") {
            return this->CategoryRender(rule, dataSource);
        } else if (renderType == "choropleth") {
            return this->ChoroplethRender(rule, dataSource);
        } else if (renderType == "RGB") {
            
        }
        return true;
    };
    
    // 分段渲染
    bool RenderBase::ChoroplethRender(rule_ptr rule, cogdatasource_ptr dataSource) {
        int width = surface_->width();
        int height = surface_->height();
        if(!rule->HasAttribute(StyleName::band)) {
            BOOST_LOG_TRIVIAL(error) << "rule don't have attribute: band";
            return false;
        }
        int band = boost::get<int>(rule->GetAttribute(StyleName::band));
        float* imgData = new float[width * height];
        if (imgData == nullptr) {
            return false;
        }
        
        dataSource->ReadRaster(width, height, band, imgData);
        if(!rule->HasAttribute(StyleName::colorMap)) {
            BOOST_LOG_TRIVIAL(error) << "rule don't have attribute: colorMap";
            delete[] imgData;
            return false;
        }
        std::vector<ColorMap> colorMapVec = boost::get<std::vector<ColorMap> >(rule->GetAttribute(StyleName::colorMap));
        uint32_t pixels[width][height];
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                pixels[i][j] = SkColorSetARGB(0, 0, 0, 0);
                
                for (int k = 0; k < colorMapVec.size(); ++k) {
                    
                    if ((imgData[i * width + j] > colorMapVec[k].value[0]) && (imgData[i * width + j] <= colorMapVec[k].value[1])) {
                        pixels[i][j] = SkColorSetARGB(255, colorMapVec[k].color[0], colorMapVec[k].color[1], colorMapVec[k].color[2]);
                        break;
                    }
                }
            }
        }
        delete[] imgData;
        SkCanvas *canvas = surface_->getCanvas();
        SkImageInfo imageInfo = SkImageInfo::MakeN32Premul(width, height);
        canvas->writePixels(imageInfo, pixels, sizeof(pixels[0]), 0, 0);
        
        return true;
    };
    
    
    // 单值渲染
    bool RenderBase::CategoryRender(rule_ptr rule, cogdatasource_ptr dataSource) {
        int width = surface_->width();
        int height = surface_->height();
        if(!rule->HasAttribute(StyleName::band)) {
            BOOST_LOG_TRIVIAL(error) << "rule don't have attribute: band";
            return false;
        }
        int band = boost::get<int>(rule->GetAttribute(StyleName::band));
        if(!rule->HasAttribute(StyleName::colorMap)) {
            BOOST_LOG_TRIVIAL(error) << "rule don't have attribute: colorMap";
            
            return false;
        }
        float* imgData = new float[width * height];
        if (imgData == nullptr) {
            return false;
        }
        
        dataSource->ReadRaster(width, height, band, imgData);
        
        std::vector<ColorMap> colorMapVec = boost::get<std::vector<ColorMap> >(rule->GetAttribute(StyleName::colorMap));
        uint32_t pixels[width][height];
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                pixels[i][j] = SkColorSetARGB(0, 0, 0, 0);
                
                for (int k = 0; k < colorMapVec.size(); ++k) {
                    
                    if (imgData[i * width + j] == colorMapVec[k].value[0]) {
                        pixels[i][j] = SkColorSetARGB(255, colorMapVec[k].color[0], colorMapVec[k].color[1], colorMapVec[k].color[2]);
                        break;
                    }
                }
            }
        }
        delete[] imgData;
        SkCanvas *canvas = surface_->getCanvas();
        SkImageInfo imageInfo = SkImageInfo::MakeN32Premul(width, height);
        canvas->writePixels(imageInfo, pixels, sizeof(pixels[0]), 0, 0);
        
        return true;
    };
    
}
