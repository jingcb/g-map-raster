#include "map.h"
#include "utils/debug_utility.h"
int main() {
    gmap::Map map(256, 256);
    MappingLog::Init(Severity::debug);
    bool test = map.Init("/Users/chenbojing/develop/g-map-raster/test/testdata/tile.json", true);
    BOOST_LOG_TRIVIAL(info)<<"result: " <<test;
    return 0;
}
