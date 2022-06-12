#include "RenderTarget.h"

namespace Application::Rendering {
    using namespace Util;

    RenderTarget::RenderTarget(Size<int> size) noexcept : size { size } {

    }

    Size<int> RenderTarget::getSize() noexcept {
        return size;
    }

}