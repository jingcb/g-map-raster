//
//  COGDataSource.cpp
//  products
//
//  Created by chenbo jing on 2019/11/18.
//

#include "COGDataSource.h"

#include "gdal.h"
#include "cpl_conv.h"
#include "cpl_string.h"
#include "cpl_error.h"
#include "gdal_utils.h"
#include "cpl_vsi.h"

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
    
    bool COGDataSource::ReadRaster(const int& width, const int& height, const int& band, void* imageData) {
        
        //GDALWarpAppOptionsForBinary psOptionsForBinary ;
        GDALAllRegister();
        char widthStr[256];
        sprintf(widthStr, "%d", width);
        char heightStr[256];
        sprintf(heightStr, "%d", height);
        
        char xmin[256];
        sprintf(xmin, "%f", xmin_);
        char ymin[256];
        sprintf(ymin, "%f", ymin_);
        char xmax[256];
        sprintf(xmax, "%f", xmax_);
        char ymax[256];
        sprintf(ymax, "%f", ymax_);
        
        char filepath[256];
        sprintf(filepath, "%s", filepath_.c_str());
        char* warpArgv[] = {"-te", xmin, ymin, xmax, ymax, "-t_srs", "epsg:3587", "-ts", widthStr, heightStr, filepath, "/vsimem/", "-q", nullptr};
        
        //int test = CSLCount(warpArgv);
        //char* warpArgv[] = {"-te", "123", "456", "150", "500", "-t_srs", "epsg:3587", "-ts", "200", "200", "filepath", "/vsimem/", "-q", nullptr};
        //GDALWarpAppOptions *psOptions = static_cast<GDALWarpAppOptions *>(
         //                                                                 CPLCalloc(1, sizeof(GDALWarpAppOptions)));
        GDALWarpAppOptions *psOptions = GDALWarpAppOptionsNew(warpArgv, nullptr);
        //CSLDestroy( warpArgv );
        GDALDatasetH *pahSrcDS = nullptr;
        pahSrcDS = static_cast<GDALDatasetH *>(CPLRealloc(pahSrcDS, sizeof(GDALDatasetH)));
        pahSrcDS[0] = GDALOpenEx( filepath_.c_str(), GDAL_OF_RASTER | GDAL_OF_VERBOSE_ERROR, nullptr, nullptr, nullptr );
        if (pahSrcDS == nullptr) {
            BOOST_LOG_TRIVIAL(error) << "can't open tiff file: " << filepath_;
            return false;
        }
        
        VSIStatBufL sStat;
        if( VSIStatExL("/vsimem/", &sStat, VSI_STAT_EXISTS_FLAG | VSI_STAT_NATURE_FLAG) == 0 &&
           S_ISFIFO(sStat.st_mode) )
        {
            
        }
        GDALDatasetH hDstDS = nullptr;
        GDALOpenEx( "/vsimem/", GDAL_OF_RASTER | GDAL_OF_VERBOSE_ERROR | GDAL_OF_UPDATE, nullptr, nullptr, nullptr );
        
        if( hDstDS != nullptr )
        {
            return false;
        }
        int bUsageError = 0;
        GDALDatasetH hOutDS = GDALWarp("/vsimem/", hDstDS,
                                       1, pahSrcDS, psOptions, &bUsageError);
        GDALRasterBandH pRasterband = GDALGetRasterBand(hOutDS, band);
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
