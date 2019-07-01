#ifndef __MAP__H__
#define __MAP__H__

namespace gmap {
    //Map类负责需要渲染的图片的所有信息的维护，比如地图的范围，分辨率。并且渲染图片
    class Map{
        public:
            Map();
            //初始化地图
            Map(int width, int hight);
            void SetBounds(double xmin, double ymin, double xmax, double ymax);
            void SetRes(double res);
            bool Render

    };
}
#endif