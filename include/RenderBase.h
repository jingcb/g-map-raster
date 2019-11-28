#ifndef __RENDER_BASE__H__
#define __RENDER_BASE__H__

#include <vector>
#include <boost/shared_ptr.hpp>
#include "include/core/SkSurface.h"

namespace gmap {
    class Rule;
    using rule_ptr = boost::shared_ptr<Rule>;
    class COGDataSource;
    using cogdatasource_ptr = boost::shared_ptr<COGDataSource>;
    class RenderBase {
    public:
        RenderBase(sk_sp<SkSurface> surface);
        ~RenderBase();
        
        bool Render(rule_ptr rule, cogdatasource_ptr dataSource);
        bool CategoryRender(rule_ptr rule, cogdatasource_ptr dataSource);
    private:
        sk_sp<SkSurface> surface_;
    };
}

#endif
