#include <node.h>
#include <nan.h>
#include "tile_map.h"
#include "utils/debug_utility.h"

#define TOSTR(obj) (*v8::String::Utf8Value((obj)->ToString()))

using namespace gmap;
NAN_METHOD(initLog) {
    // default print detail log
    int severity = 5;
    
    // default print to console
    std::string log_folder = "";
    
    if (info.Length() >= 1) {
        severity  = info[0]->IntegerValue();
    }
    
    if (info.Length() >= 2) {
        log_folder = TOSTR(info[1]);
    }
    
    MappingLog::Init(static_cast<Severity>(severity), log_folder);
    
    return;
}
struct Baton {
    uv_work_t request;
    Nan::Persistent<v8::Function> callback;
    
    std::string message;
    std::string stream;
    
    unsigned int x, y, z;
    std::string style;
    bool fromfile;
    unsigned int retina;
    
    Baton() :
    x(0), y(0), z(0),
    fromfile(false),
    retina(1) {
        this->request.data = this;
    }
    
    ~Baton() {
        this->callback.Reset();
    }
};

void TileWorker(uv_work_t* req) {
    Baton* baton = static_cast<Baton*>(req->data);
    TileMap tileMap(baton->z, baton->x, baton->y);
    if (!tileMap.Tile(baton->style, baton->fromfile, baton->retina)) {
        baton->message = "v8: tie fail!";
        return;
    }
    if (!tileMap.SaveStream(baton->stream)) {
        baton->message = "v8: save fail!";
        return;
    }
    return;
}

void TileWorkerAfter(uv_work_t* req) {
    Nan::HandleScope scope;
    Baton* baton = static_cast<Baton*>(req->data);
    
    if (!baton->message.length()) {
        
        v8::Local<v8::Value> argv[] = {
            Nan::Null(),
            Nan::CopyBuffer(baton->stream.data(), baton->stream.length()).ToLocalChecked()
        };
        
        Nan::MakeCallback(Nan::GetCurrentContext()->Global(), Nan::New(baton->callback), 2, argv);
        
    } else {
        v8::Local<v8::Value> argv[] = {
            Nan::Error(baton->message.c_str())
        };
        Nan::MakeCallback(Nan::GetCurrentContext()->Global(), Nan::New(baton->callback), 1, argv);
    }
    delete baton;
}



NAN_METHOD(tile) {
    if (info.Length() < 2 || !info[0]->IsObject() || !info[1]->IsFunction()) {
        Nan::ThrowError("Must has two argument! object and function");
        return;
    }
    std::unique_ptr<Baton> baton(new Baton());
    v8::Local<v8::Object> options;
    options = v8::Local<v8::Object>::Cast(info[0]);
    baton->callback.Reset(info[1].As<v8::Function>());
    
    if (!options->Has(Nan::New("style").ToLocalChecked())) {
        Nan::ThrowTypeError("Must has style options");
        return;
    }
    
    v8::Local<v8::Value> style = options->Get(Nan::New("style").ToLocalChecked());
    baton->style = *v8::String::Utf8Value(style);
    
    if (options->Has(Nan::New("fromfile").ToLocalChecked())) {
        baton->fromfile = options->Get(Nan::New("fromfile").ToLocalChecked())->BooleanValue();
    }
    if (options->Has(Nan::New("x").ToLocalChecked())) {
        baton->x = options->Get(Nan::New("x").ToLocalChecked())->Uint32Value();
    }
    if (options->Has(Nan::New("y").ToLocalChecked())) {
        baton->y = options->Get(Nan::New("y").ToLocalChecked())->Uint32Value();
    }
    if (options->Has(Nan::New("z").ToLocalChecked())) {
        baton->z = options->Get(Nan::New("z").ToLocalChecked())->Uint32Value();
    }
    if (options->Has(Nan::New("retina").ToLocalChecked())) {
        baton->retina = options->Get(Nan::New("retina").ToLocalChecked())->Uint32Value();
    }
    
    uv_queue_work(uv_default_loop(), &(baton.release())->request, TileWorker, (uv_after_work_cb)TileWorkerAfter);
    return;
    
}


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
    Nan::SetMethod(target, "tile", tile);
    // Nan::SetMethod(target, "staticMap", staticMap);
    // Nan::SetMethod(target, "dataThumb", dataThumb);
    // Nan::SetMethod(target, "layout", layout);
    
    // Nan::SetMethod(target, "renderextent", renderextent);
    
    //Map::Initialize(target);
}

NODE_MODULE(gmap, initAll)
