# g-map-raster
按照设定好的风格配置渲染栅格数据
## 渲染风格（配置文件）(json)
```
{
    datasetDir: "...",
    layers: {
        "layer1": {
            data: "...",
            rule: {
                "renderType": "...".
                "renderbands": [], 
                "colorMap": [
                    {
                        "value": 20,
                        "color": [0,255,255],
                        "
                    }
                ]
            }
        }
    }
}
```
