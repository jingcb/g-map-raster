#include "tile_map.h"
#include "utils/debug_utility.h"
#include <boost/timer/timer.hpp>
int main() {
    MappingLog::Init(Severity::debug);
    boost::timer::auto_cpu_timer timer("raphicSink::Record: %w clock time,%t totle program time\n");
    unsigned int z =
    unsigned int x =
    unsigned int y =
    TileMap tile();
    
    if(!tile.Tile("/Users/chenbojing/develop/g-map-raster/test/testdata/Category.json", true, 2)) {
        BOOST_LOG_TRIVIAL(error) << "Init fail";
    }
    const std::string outfile = "/Users/chenbojing/develop/g-map-raster/test/out/category.png";
    if (!tile.SaveFile(outfile)) {
        BOOST_LOG_TRIVIAL(error) << "save fail";
    }
    return 0;
}
