#include "tile_map.h"
#include "utils/debug_utility.h"
#include <boost/timer/timer.hpp>
int main() {
    MappingLog::Init(Severity::debug, "/Users/chenbojing/develop/g-map-raster/test/testdata/test.log");
    boost::timer::auto_cpu_timer timer("raphicSink::Record: %w clock time,%t totle program time\n");
    unsigned int z = 11;
    unsigned int x = 481;
    unsigned int y = 756;
    gmap::TileMap tile(z, x, y);
    
    if(!tile.Tile("/Users/chenbojing/develop/g-map-raster/test/testdata/Category.json", true, 2)) {
        BOOST_LOG_TRIVIAL(error) << "Init fail";
    }
    const std::string outfile = "/Users/chenbojing/develop/g-map-raster/test/out/category.png";
    if (!tile.SaveFile(outfile)) {
        BOOST_LOG_TRIVIAL(error) << "save fail";
    }
    return 0;
}
