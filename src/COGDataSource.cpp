//
//  COGDataSource.cpp
//  products
//
//  Created by chenbo jing on 2019/11/18.
//

#include "COGDataSource.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "cpl_string.h"
#include "cpl_error.h"
#include "gdal_utils.h"
#include "cpl_vsi.h"

#include "utils/debug_utility.h"

#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"

namespace gmap {
    
    COGDataSource::COGDataSource() : filepath_("") {
        GDALAllRegister();
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
        BOOST_LOG_TRIVIAL(info) << "start get data";
        
        
        char widthStr[256];
        sprintf(widthStr, "%d", width);
        char heightStr[256];
        sprintf(heightStr, "%d", height);
        
        char xmin[256];
        sprintf(xmin, "%.4f", xmin_);
        char ymin[256];
        sprintf(ymin, "%.4f", ymin_);
        char xmax[256];
        sprintf(xmax, "%.4f", xmax_);
        char ymax[256];
        sprintf(ymax, "%.4f", ymax_);
        
        char filepath[256];
        sprintf(filepath, "%s", filepath_.c_str());
        boost::uuids::random_generator  uuid_v4;
        
        //std::string warp
        char* warpArgv[] = {"-te", xmin, ymin, xmax, ymax, "-t_srs", "epsg:3857", "-ts", widthStr, heightStr, filepath,  "-q", nullptr};
        
        //int test = CSLCount(warpArgv);
        //char* warpArgv[] = {"-te", "123", "456", "150", "500", "-t_srs", "epsg:3587", "-ts", "200", "200", "filepath", "/vsimem/", "-q", nullptr};
        //GDALWarpAppOptions *psOptions = static_cast<GDALWarpAppOptions *>(
         //                                                                 CPLCalloc(1, sizeof(GDALWarpAppOptions)));
        
        GDALWarpAppOptions *psOptions = GDALWarpAppOptionsNew(warpArgv, nullptr);
        //CSLDestroy( warpArgv );
        GDALDatasetH *pahSrcDS = nullptr;
        BOOST_LOG_TRIVIAL(info) << "start realloc" << filepath_;
        pahSrcDS = static_cast<GDALDatasetH *>(CPLRealloc(pahSrcDS, sizeof(GDALDatasetH)));
        pahSrcDS[0] = GDALOpenEx( filepath_.c_str(), GDAL_OF_RASTER | GDAL_OF_VERBOSE_ERROR, nullptr, nullptr, nullptr );
        if (pahSrcDS[0] == nullptr) {
            BOOST_LOG_TRIVIAL(error) << "can't open tiff file: " << filepath_;
            return false;
        }
        
       
        
        std::string memFileName = "/vsimem/" + boost::uuids::to_string(uuid_v4()) + ".tif";
        
        int bUsageError = 0;
        
        GDALDatasetH hOutDS = GDALWarp(memFileName.c_str(), nullptr,
                                       1, pahSrcDS, psOptions, &bUsageError);
        GDALRasterBandH pRasterband = GDALGetRasterBand(hOutDS, band);
        if(GDALRasterIO(pRasterband, GF_Read, 0, 0, width, height, imageData, width, height, GDT_Float32, 0, 0) == CPLErr::CE_Failure) {
            return false;
        }
        BOOST_LOG_TRIVIAL(info) << "start delete mem tiff";
        
        if (hOutDS != nullptr) {
            
            //OGRReleaseDataSource(hOutDS);
            std::string driverName = "Memory";
            GDALDriverH driver = GDALGetDriverByName(driverName.c_str());
            if(GDALDeleteDataset(driver, memFileName.c_str()) == CE_Failure) {
                BOOST_LOG_TRIVIAL(warning) << "can't delete memory file: " << memFileName;
            }
            GDALClose(hOutDS);
        } else {
            std::cout<<"no dataset"<<std::endl;
        }
        
        BOOST_LOG_TRIVIAL(info) << "start delete optionsb";
        GDALWarpAppOptionsFree(psOptions);
        GDALClose(pahSrcDS[0]);
        CPLFree(pahSrcDS);
        //GDALDestroyDriverManager();
        BOOST_LOG_TRIVIAL(info) << "sucessful get data";
        //OGRCleanupAll();
        
        return true;
    }
    
}
