#include "WindowStyle.h"

#include <type_traits>

namespace Application::Window {

    WindowStyle& operator|(WindowStyle lhs, const WindowStyle& rhs) {
        lhs = (WindowStyle) (
            static_cast<std::underlying_type_t<WindowStyle>>(lhs)
            | static_cast<std::underlying_type_t<WindowStyle>>(rhs));

        return lhs;
    }

}