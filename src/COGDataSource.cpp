//
//  COGDataSource.cpp
//  products
//
//  Created by chenbo jing on 2019/11/18.
//

#include "COGDataSource.h"

#include "gdal.h"
#include "cpl_conv.h"
#include "gdal_utils.h"

#include "utils/debug_utility.h"

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
        
        //GDALWarpAppOptionsForBinary psOptionsForBinary ;
        char* widthStr;
        sprintf(widthStr, "%d", width);
        char* heightStr;
        sprintf(heightStr, "%d", height);
        
        char* xmin;
        sprintf(xmin, "%f", xmin_);
        char* ymin;
        sprintf(ymin, "%f", ymin_);
        char* xmax;
        sprintf(xmax, "%f", xmax_);
        char* ymax;
        sprintf(ymax, "%f", ymax_);
        
        char* filepath;
        sprintf(filepath, "%s", filepath_.c_str());
        char* warpArgv[] = {"-te", xmin, ymin, xmax, ymax, "-t_srs", "epsg:3587", "-ts", widthStr, heightStr, filepath, "/vsimem/", "-q"};
        GDALWarpAppOptions *psOptions = GDALWarpAppOptionsNew(warpArgv, nullptr);
        GDALDatasetH pahSrcDS[] = {GDALOpenEx( filepath_.c_str(), GDAL_OF_RASTER | GDAL_OF_VERBOSE_ERROR, nullptr, nullptr, nullptr )};
        if (pahSrcDS == nullptr) {
            BOOST_LOG_TRIVIAL(error) << "can't open tiff file: " << filepath_;
            return false;
        }
        
        
        GDALDatasetH hDstDS = GDALOpenEx( "/vsimem/", GDAL_OF_RASTER | GDAL_OF_VERBOSE_ERROR | GDAL_OF_UPDATE, nullptr, nullptr, nullptr );
        
        if( hDstDS != nullptr )
        {
            return false;
        }
        int bUsageError = 0;
        GDALDatasetH hOutDS = GDALWarp("/vsimem/", hDstDS,
                                       1, pahSrcDS, psOptions, &bUsageError);
        GDALRasterBandH pRasterband = GDALGetRasterBand(hOutDS, 1);
        if(GDALRasterIO(pRasterband, GF_Read, 0, 0, width, height, imageData, width, height, GDT_Float32, 0, 0) == CPLErr::CE_Failure) {
            return false;
        }
        
        GDALClose( hOutDS ? hOutDS : hDstDS );
        GDALWarpAppOptionsFree(psOptions);
        GDALClose(pahSrcDS[0]);
        CPLFree(pahSrcDS);
        
        GDALDumpOpenDatasets( stderr );
        
        GDALDestroyDriverManager();
        
        OGRCleanupAll();
        
        return true;
    }
    
}
