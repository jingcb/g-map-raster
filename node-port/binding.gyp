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
          "./lib/librastermap.a"
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
