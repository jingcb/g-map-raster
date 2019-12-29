var gmap = require("../node-port/build/Build/Products/Release/gmap.node")
var fs = require("fs")
gmap.initLog(5, "/Users/chenbojing/develop/g-map-raster/test/log")

var style = {
    "datasetDir": "/Users/chenbojing/data/raster",
    "layers": {
        "layer1": {
            "data": "2017_corp.tif",
            "rules": [{
                "renderType": "category",
                "bands": {
                    "r": 1,
                    "g": 2,
                    "b": 3
                },
                "band": 1,
                "colorMap": [{
                        "value": 0,
                        "color": [0, 255, 0]
                    },
                    {
                        "value": 1,
                        "color": [0, 0, 255]
                    },
                    {
                        "value": 5,
                        "color": [255, 0, 0]
                    }
                ],
                "extent": {
                    "b1": [15, 20],
                    "b2": [35, 40]
                }
            }]


        }
    }
}
let params = {
    "z": 11,
    "x": 481,
    "y": 756,
    "retina": 2,
    "style": JSON.stringify(style),
    "fromfile": false
}

gmap.tile(params, function (err, stream) {
    fs.appendFileSync("/Users/chenbojing/develop/g-map-raster/test/out/nodetest.png", stream)
})