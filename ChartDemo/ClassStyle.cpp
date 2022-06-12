#include "ClassStyle.h"

#include <type_traits>

namespace Application::Window {

    ClassStyle& operator|(ClassStyle lhs, const ClassStyle& rhs) {
        lhs = (ClassStyle) (
            static_cast<std::underlying_type_t<ClassStyle>>(lhs)
            | static_cast<std::underlying_type_t<ClassStyle>>((unsigned) rhs));

        return lhs;
    }

}