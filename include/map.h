#ifndef __MAP__H__
#define __MAP__H__
#include <iostream>
#include <boost/shared_ptr.hpp>
namespace gmap {
    class MapStyle;
    using mapstyle_ptr = boost::shared_ptr<MapStyle>;
    //Map类负责需要渲染的图片的所有信息的维护，比如地图的范围，分辨率。并且渲染图片
    class Map{
    public:
        Map();
        //初始化地图
        Map(int width, int height);
        ~Map();
        void SetBounds(double xmin, double ymin, double xmax, double ymax);
        void SetRes(double res);
        bool Render();
        bool Init(const std::string &def);
    protected:
        int width_; //需要渲染的图片的宽（像素）
        int height_; //需要渲染的图片的高（像素
        double xmin_;
        double ymin_;
        double xmax_;
        double ymax_;
        double res_;
        mapstyle_ptr mapStyle_;
        
    };
}
#endif
