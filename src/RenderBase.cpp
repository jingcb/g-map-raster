#include "RenderBase.h"

#include "rule.h"

namespace gmap {
    RenderBase::RenderBase(sk_sp<SkSurface> surface) : surface_(surface) {
        
    };
    RenderBase::~RenderBase() {
        
    }
}
