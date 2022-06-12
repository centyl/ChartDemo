#include "ButtonStyle.h"

#include <type_traits>

namespace Application::Window {

    ButtonStyle& operator|(ButtonStyle lhs, const ButtonStyle& rhs) {
        lhs = (ButtonStyle) (
            static_cast<std::underlying_type_t<ButtonStyle>>(lhs)
            | static_cast<std::underlying_type_t<ButtonStyle>>(rhs));

        return lhs;
    }

}