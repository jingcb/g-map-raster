#include "map.h"
#include "utils/debug_utility.h"
#include <boost/timer/timer.hpp>
int main() {
    MappingLog::Init(Severity::debug);
    boost::timer::auto_cpu_timer timer("raphicSink::Record: %w clock time,%t totle program time\n");
    gmap::Map map(512, 512);
    map.SetBounds(-10587715.8, 5162143.6, -10580576.9, 5171265.5);
    if(!map.Init("/Users/chenbojing/develop/g-map-raster/test/testdata/Category.json", true)) {
        BOOST_LOG_TRIVIAL(error) << "Init fail";
    }
    if (!map.Render()) {
        BOOST_LOG_TRIVIAL(error) << "Render fail";
    }
    const std::string outfile = "/Users/chenbojing/develop/g-map-raster/test/out/category.png";
    if (!map.SaveFile(outfile)) {
        BOOST_LOG_TRIVIAL(error) << "save fail";
    }
    return 0;
}
