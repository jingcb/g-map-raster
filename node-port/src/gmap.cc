#include <node.h>
#include <nan.h>
#include "tile_map.h"
static void initAll(v8::Handle<v8::Object> target) {
    Nan::HandleScope scope;
    
    // module level functions
    // Nan::SetMethod(target, "initLog", initLog);
    // Nan::SetMethod(target, "registerFonts", registerFonts);
    // Nan::SetMethod(target, "registerPool", registerPool);
    // Nan::SetMethod(target, "registerStorage", registerStorage);
    // Nan::SetMethod(target, "addMBTiler", addMBTiler);
    // Nan::SetMethod(target, "createMBTiler", createMBTiler);
    // Nan::SetMethod(target, "getFromMbtiler", getFromMbtiler);
    
    // Nan::SetMethod(target, "getFile", getFile);
    // Nan::SetMethod(target, "tile", tile);
    // Nan::SetMethod(target, "staticMap", staticMap);
    // Nan::SetMethod(target, "dataThumb", dataThumb);
    // Nan::SetMethod(target, "layout", layout);
    
    // Nan::SetMethod(target, "renderextent", renderextent);
    
    //Map::Initialize(target);
}

NODE_MODULE(gmap, initAll)
