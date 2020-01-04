#include "tile_map.h"

#include "utils/debug_utility.h"
#include <boost/timer/timer.hpp>
#include <unistd.h>
int main() {

    MappingLog::Init(Severity::info);
    boost::timer::auto_cpu_timer timer("raphicSink::Record: %w clock time,%t totle program time\n");
    unsigned int z = 11;
    unsigned int x = 481;
    unsigned int y = 756;
    
    for (int i = 1; i < 100; i++) {
        if (i==100) {
            sleep(2);
        }
        gmap::TileMap tile1(z, x, y);
        gmap::TileMap tile2(z, x, y);
    if(!tile1.Tile("/Users/chenbojing/develop/g-map-raster/test/testdata/Category.json", true, 2)) {
        BOOST_LOG_TRIVIAL(error) << "Init fail";
    }
        if(!tile2.Tile("/Users/chenbojing/develop/g-map-raster/test/testdata/Category.json", true, 2)) {
            BOOST_LOG_TRIVIAL(error) << "Init fail";
        }
    const std::string outfile = "/Users/chenbojing/develop/g-map-raster/test/out/category.png";
    if (!tile1.SaveFile(outfile)) {
        BOOST_LOG_TRIVIAL(error) << "save fail";
    }}
    
    return 0;
}
