#pragma once

#include "Point.h"

namespace Application::Util {

    template <typename T>
    struct Line {
        Application::Util::Point<T> firstPoint;
        Application::Util::Point<T> secondPoint;
    };

}

