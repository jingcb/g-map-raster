#ifndef __RENDER_BASE__H__
#define __RENDER_BASE__H__

#include <vector>
#include <boost/shared_ptr.hpp>
#include "include/core/SkSurface.h"

namespace gmap {
    class Rule;
    using rule_ptr = boost::shared_ptr<Rule>;
    class RenderBase {
    public:
        RenderBase(sk_sp<SkSurface> surface);
        ~RenderBase();
        
        bool Render(rule_ptr rule, float* const imageData, const int& width, const int& height);
        bool CategoryRender(const std::vector<double>& valueVec, const std::vector<std::vector<int>>& colorVec, const int& width, const int& height, float* const imageData);
    private:
        sk_sp<SkSurface> surface_;
    };
}

#endif
