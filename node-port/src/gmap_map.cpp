
#include "gmap_map.h"
#include "tile_map.h"

#define TOSTR(obj) (*v8::String::Utf8Value((obj)->ToString()))

Nan::Persistent<v8::FunctionTemplate> Map::constructor;

void Map::Initialize(v8::Local<v8::Object> target) {
    Nan::HandleScope scope;
    
    v8::Local<v8::FunctionTemplate> lcons = Nan::New<v8::FunctionTemplate>(Map::New);
    lcons->InstanceTemplate()->SetInternalFieldCount(1);
    lcons->SetClassName(Nan::New("Map").ToLocalChecked());
    
   
    Nan::SetPrototypeMethod(lcons, "tile", tile);
    
    target->Set(Nan::New("Map").ToLocalChecked(),lcons->GetFunction());
    constructor.Reset(lcons);
}

Map::Map() :
Nan::ObjectWrap(),
in_use_(false) {
}

Map::~Map() {}

bool Map::acquire() {
    if (in_use_) {
        return false;
    }
    in_use_ = true;
    return true;
}

void Map::release() {
    in_use_ = false;
}

NAN_METHOD(Map::New) {
    if (!info.IsConstructCall()) {
        Nan::ThrowError("Cannot call constructor as function, you need to use 'new' keyword");
        return;
    }
    
    // accept a reference or v8:External?
    if (info[0]->IsExternal()) {
        v8::Local<v8::External> ext = info[0].As<v8::External>();
        void* ptr = ext->Value();
        Map* m =  static_cast<Map*>(ptr);
        m->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
        return;
    }
    
    Map* m = new Map();
    m->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
    return;
}


//typedef struct {
//    uv_work_t request;
//    Map *m;
//
//    unsigned int x, y, z;
//    std::string style;
//    bool fromfile;
//    unsigned int retina;
//    bool error;
//    std::string error_name;
//    Nan::Persistent<v8::Function> cb;
//} init_baton_t;
//
//NAN_METHOD(Map::init) {
//    if (info.Length() < 2) {
//        Nan::ThrowError("Need two arguments: options and callback");
//        return;
//    }
//
//    // ensure callback is a function
//    v8::Local<v8::Value> callback = info[info.Length() - 1];
//    if (!callback->IsFunction()) {
//        Nan::ThrowTypeError("last argument must be a callback function");
//        return;
//    }
//
//    // ensure options object
//    if (!info[0]->IsObject()) {
//        Nan::ThrowTypeError("options must be an object, eg {fromFile: true}");
//        return;
//    }
//
//    v8::Local<v8::Object> options = info[0].As<v8::Object>();
//
//    bool from_file = false;
//    v8::Local<v8::String> param = Nan::New("fromFile").ToLocalChecked();
//    if (options->Has(param)) {
//        v8::Local<v8::Value> param_val = options->Get(param);
//        if (!param_val->IsBoolean()) {
//            Nan::ThrowTypeError("'fromFile' must be a Boolean");
//            return;
//        }
//        from_file = param_val->BooleanValue();
//    }
//
//    Map* m = Nan::ObjectWrap::Unwrap<Map>(info.Holder());
//
//    init_baton_t *closure = new init_baton_t();
//    closure->request.data = closure;
//
//    v8::Local<v8::Value> map_dir = options->Get(Nan::New("mapDir").ToLocalChecked());
//    v8::Local<v8::Value> lod = options->Get(Nan::New("lod").ToLocalChecked());
//    v8::Local<v8::Value> style = options->Get(Nan::New("style").ToLocalChecked());
//
//    closure->base_path = *v8::String::Utf8Value(map_dir);
//    closure->lod = *v8::String::Utf8Value(lod);
//    closure->def = *v8::String::Utf8Value(style);
//    closure->from_file = from_file;
//    closure->m = m;
//    closure->error = false;
//    closure->cb.Reset(callback.As<v8::Function>());
//
//    uv_queue_work(uv_default_loop(), &closure->request, EIO_Init, (uv_after_work_cb)EIO_AfterInit);
//
//    m->Ref();
//
//    return;
//}
//
//void Map::EIO_Init(uv_work_t* req) {
//    init_baton_t *closure = static_cast<init_baton_t*>(req->data);
//
//    if (!closure->m->map_->Init(closure->base_path, closure->lod, closure->def, closure->from_file)) {
//        closure->error = true;
//        closure->error_name = "Init fail!";
//    }
//}
//
//void Map::EIO_AfterInit(uv_work_t* req) {
//    Nan::HandleScope scope;
//    init_baton_t *closure = static_cast<init_baton_t *>(req->data);
//    if (closure->error) {
//        v8::Local<v8::Value> argv[1] = { Nan::Error(closure->error_name.c_str()) };
//        Nan::MakeCallback(Nan::GetCurrentContext()->Global(), Nan::New(closure->cb), 1, argv);
//    } else {
//        v8::Local<v8::Value> argv[2] = { Nan::Null(), closure->m->handle() };
//        Nan::MakeCallback(Nan::GetCurrentContext()->Global(), Nan::New(closure->cb), 2, argv);
//    }
//
//    closure->m->Unref();
//    closure->cb.Reset();
//    delete closure;
//}


typedef struct {
    uv_work_t request;
    Map *m;
    
    unsigned int x, y, z;
    std::string style;
    bool fromfile;
    unsigned int retina;
    std::string data;
    
    bool error;
    std::string error_name;
    Nan::Persistent<v8::Function> cb;
} tile_baton_t;


NAN_METHOD(Map::tile) {
    if (info.Length() < 2) {
        Nan::ThrowError("Need two arguments: options and callback");
        return;
    }
    
    // ensure callback is a function
    v8::Local<v8::Value> callback = info[info.Length() - 1];
    if (!callback->IsFunction()) {
        Nan::ThrowTypeError("last argument must be a callback function");
        return;
    }
    
    // ensure options object
    if (!info[0]->IsObject()) {
        Nan::ThrowTypeError("options must be an object, eg {renderType: 0}");
        return;
    }
    
    v8::Local<v8::Object> options = info[0].As<v8::Object>();
    Map* m = Nan::ObjectWrap::Unwrap<Map>(info.Holder());
    
    tile_baton_t *closure = new tile_baton_t();
    
    closure->request.data = closure;
    closure->m = m;
    
    v8::Local<v8::Value> style = options->Get(Nan::New("style").ToLocalChecked());
    closure->style = *v8::String::Utf8Value(style);
    
    if (options->Has(Nan::New("z").ToLocalChecked())) {
        closure->z = options->Get(Nan::New("z").ToLocalChecked())->Int32Value();
    }
    
    if (options->Has(Nan::New("x").ToLocalChecked())) {
        closure->x = options->Get(Nan::New("x").ToLocalChecked())->Int32Value();
    }
    
    if (options->Has(Nan::New("y").ToLocalChecked())) {
        closure->y = options->Get(Nan::New("y").ToLocalChecked())->Int32Value();
    }
    
    m->map_ = std::make_shared<gmap::TileMap>(closure->z, closure->x, closure->y);
    
    if (options->Has(Nan::New("retina").ToLocalChecked())) {
        closure->retina = options->Get(Nan::New("retina").ToLocalChecked())->Int32Value();
    }
    
  
    closure->error = false;
    closure->cb.Reset(callback.As<v8::Function>());
    uv_queue_work(uv_default_loop(), &closure->request, EIO_Tile, (uv_after_work_cb)EIO_AfterTile);
    m->Ref();
    
    return;
}



void Map::EIO_Tile(uv_work_t* req) {
    tile_baton_t *closure = static_cast<tile_baton_t*>(req->data);
    if (!closure->m->map_->Tile(closure->style, closure->fromfile, closure->retina)) {
        closure->error = true;
        closure->error_name = "Tile error!";
    } else {
        closure->m->map_->SaveStream(closure->data);
    }
}

void Map::EIO_AfterTile(uv_work_t* req) {
    Nan::HandleScope scope;
    tile_baton_t *closure = static_cast<tile_baton_t *>(req->data);
    closure->m->release();
    
    if (closure->error) {
        v8::Local<v8::Value> argv[1] = { Nan::Error(closure->error_name.c_str()) };
        Nan::MakeCallback(Nan::GetCurrentContext()->Global(), Nan::New(closure->cb), 1, argv);
    } else {
        v8::Local<v8::Value> argv[2] = { Nan::Null(), Nan::CopyBuffer(closure->data.data(), closure->data.length()).ToLocalChecked() };
        Nan::MakeCallback(Nan::GetCurrentContext()->Global(), Nan::New(closure->cb), 2, argv);
    }
    
    closure->m->Unref();
    closure->cb.Reset();
    delete closure;
}
