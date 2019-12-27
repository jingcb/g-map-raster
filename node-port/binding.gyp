{
  "targets": [
    {
      "target_name": "gmap",
      'cflags_cc': 
            [
                '-std=c++11',
                '-frtti',
                '-fexceptions',
            ],
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