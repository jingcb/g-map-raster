
#ifndef __gmap__map__hh__
#define __gmap__map__hh__

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wshadow"
#include <nan.h>
#pragma GCC diagnostic pop

// stl
#include <string>
#include <iostream>
#include <memory>



namespace gmap { class TileMap; }

typedef std::shared_ptr<gmap::TileMap> map_ptr;

class Map: public Nan::ObjectWrap {
public:
    
    static Nan::Persistent<v8::FunctionTemplate> constructor;
    static void Initialize(v8::Local<v8::Object> target);
    static NAN_METHOD(New);
    
    static NAN_METHOD(init);
    static void EIO_Init(uv_work_t* req);
    static void EIO_AfterInit(uv_work_t* req);
    
    static NAN_METHOD(tile);
    static void EIO_Tile(uv_work_t* req);
    static void EIO_AfterTile(uv_work_t* req);
    
    
    
    Map();
    ~Map();
    
    bool acquire();
    void release();
    void _ref() { Ref(); }
    void _unref() { Unref(); }
    
    inline map_ptr get() { return map_; }
    
private:
    
    bool in_use_;
    map_ptr map_;
    
};

#endif
