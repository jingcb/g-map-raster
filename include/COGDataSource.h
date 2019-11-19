#ifndef __COG_DATASOURCE_H__
#define __COG_DATASOURCE_H__

#include <string>
namespace gmap {
    class COGDataSource {
    public:
        COGDataSource();
        ~COGDataSource();
        void SetSpitialFilter(double xmin, double ymin, double xmax, double ymax);
        void SetDataPath(const std::string& filepath) { filepath_ = filepath;}
        bool RasterIO(const int& width, const int& height);
        bool ReadRaster(const int& width, const int& height, void* imageData);
    private:
        std::string filepath_;
        double xmin_;
        double ymin_;
        double xmax_;
        double ymax_;
    };
}


#endif /* COGDataSource_hpp */
