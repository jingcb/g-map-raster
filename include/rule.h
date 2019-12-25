#ifndef __RULE__H__
#define __RULE__H__
#include "boost/variant.hpp"


#include <map>
#include <string>
#include <vector>
namespace gmap {
    enum class StyleTypeName {
        INT,
        DOUBLE,
        STRING,
        VECTOR,     // {1, 2, 3}
        DOUBLEVECTOR, // {0.2, 0.3, 0.4}
        VECTOROFVECTOR, // {{1, 2, 3}, {4, 5, 6}, ...}
        MAP,
        MAPOFVECTOR,
        VECTOROFCOLORMAP
    };
    
    struct ColorMap {
        std::vector<double> value;
        
        std::vector<int> color;
    };
    
    // 属性变量类型
    using StyleType = boost::variant<int, double, std::string, std::vector<int>, std::vector<double>, std::vector<std::vector<int> >, std::map<std::string, int>,  std::map<std::string, std::vector<double> >, std::vector<ColorMap> >;
    // 属性的描述
    struct TypeDesc {
        // 名称
        std::string name_;
        
        // 变量类型名称
        StyleTypeName type_;
        
        // 构造函数
        TypeDesc(std::string name, StyleTypeName type) {
            name_ = name;
            type_ = type;
        }
    };
    
    // 所有可用的属性名称
    enum class StyleName {
        //RGB Render
        bands, extent, nodata,
        //Categrory Render
        band, colorMap,
        renderType
    };
    // the rule to render map !
    class Rule {
    public:
        using attribute_map = std::map<StyleName, StyleType>;
        Rule();
        ~Rule();
        
        Rule& operator=(Rule rhs);
        /** 是否包含某个属性 */
        bool HasAttribute(StyleName key);
        
        /** 根据属性名称获得属性值 */
        StyleType GetAttribute(StyleName key);
        
        /** 设置属性, 若无则插入 */
        void SetAttribute(StyleName key, StyleType style);
    private:
        attribute_map attributes_;
    };
}
#endif
