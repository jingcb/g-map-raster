{
  "targets": [
    {
      "target_name": "gmap",
      'cflags_cc': 
            [
                '-std=c++14',
                '-frtti',
                '-fexceptions',
            ],
      "xcode_settings":
      {
          "CLANG_CXX_LIBRARY": "libc++",
          "GCC_ENABLE_CPP_RTTI": "YES",
          "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
      },
      "sources": [
        "./src/gmap.cc"
      ],
      "libraries": [
          "./lib/librastermap.a",
          "../3rd/skia/lib/libskia.a",
          "../3rd/skia/lib/libpathkit.a",
          "../3rd/skia/lib/libmodules__particles.a",
          "../3rd/skia/lib/libmodules__skottie.a",
          "../3rd/skia/lib/libmodules__sksg.a",
          "../3rd/skia/lib/libmodules__skshaper.a",
          "../3rd/skia/lib/libthird_party__dng_sdk.a",
          "../3rd/skia/lib/libthird_party__expat.a",
          "../3rd/skia/lib/libthird_party__harfbuzz.a",
          "../3rd/skia/lib/libthird_party__icu.a",
          "../3rd/skia/lib/libthird_party__libjpeg-turbo_libjpeg.a",
          "../3rd/skia/lib/libthird_party__libpng.a",
          "../3rd/skia/lib/libthird_party__libwebp_libwebp_sse41.a",
          "../3rd/skia/lib/libthird_party__libwebp.a",
          "../3rd/skia/lib/libthird_party__piex.a",
          "../3rd/skia/lib/libthird_party__sfntly.a",
          "../3rd/skia/lib/libthird_party__spirv-tools.a",
          "../3rd/skia/lib/libthird_party__zlib_zlib_x86.a",
          "../3rd/skia/lib/libthird_party__zlib.a",
          "AppKit.framework",
          "ApplicationServices.framework",
          "OpenGL.framework",
      ],
      "include_dirs": [
          "<!(node -e \"require('nan')\")",
          "../include/",
          "/usr/local/include/",
          "../3rd/skia"
      ]
      
    }
  ]
}
