#include "ExtendedWindowStyle.h"

#include <type_traits>

namespace Application::Window {

    ExtendedWindowStyle& operator|(ExtendedWindowStyle lhs, const ExtendedWindowStyle& rhs) {
        lhs = (ExtendedWindowStyle) (
            static_cast<std::underlying_type_t<ExtendedWindowStyle>>(lhs)
            | static_cast<std::underlying_type_t<ExtendedWindowStyle>>(rhs));

        return lhs;
    }

}