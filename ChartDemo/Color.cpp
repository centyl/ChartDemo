#include "Color.h"

namespace Application::Util {

    Color::Color(float r, float g, float b, float a) noexcept : r { r }, g { g }, b { b }, a { a } {

    }

    Color::Color(int r, int g, int b, int a)  noexcept
        : r { (float) r / 255 },
        g { (float) g / 255 },
        b { (float) b / 255 },
        a { (float) a / 255 } {

    }

}