#ifndef __MAP__H__
#define __MAP__H__
#include <iostream>
#include "boost/shared_ptr.hpp"
#include <include/core/SkBitmap.h>
#include "include/core/SkSurface.h"
#include "COGDataSource.h"
namespace gmap {
    class MapStyle;
    using mapstyle_ptr = boost::shared_ptr<MapStyle>;
    class COGDataSource;
    using cogdatasource_ptr = boost::shared_ptr<COGDataSource>;
    // Map类负责需要渲染的图片的所有信息的维护，比如地图的范围，分辨率。并且渲染图片
    class Map{
    public:
        // 初始化地图
        Map();
        Map(int width, int height);
        ~Map();
        
        // 初始化渲染风格
        bool Init(const std::string &def, bool fromFile);
        
        void SetBounds(const double &xmin, const double &ymin, const double &xmax, const double &ymax);
        void SetRes(const double &res);
        void SetWidth(const int &width) {width_ = width;};
        void SetHeight(const int &height) {height_ = height;};
        
        bool Render();
        bool SaveFile(const std::string filePath);

        inline double xmin() { return xmin_; };
        inline double ymin() { return ymin_; };
        inline double xmax() { return xmax_; };
        inline double ymax() { return ymax_; };
    protected:
        int width_; //需要渲染的图片的宽（像素）
        int height_; //需要渲染的图片的高（像素
        double xmin_;
        double ymin_;
        double xmax_;
        double ymax_;
        double res_;
        mapstyle_ptr mapStyle_;
        SkBitmap bitmap_;
        sk_sp<SkSurface> surface_;
        cogdatasource_ptr cogDataSource;
    };
}
#endif
