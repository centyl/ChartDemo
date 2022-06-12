#pragma once

#include <Windows.h>
#include <stdexcept>

#include "WindowStyle.h"
#include "ExtendedWindowStyle.h"

namespace Application::Util {

    template <typename T>
    struct Size {
        T width;
        T height;
    };

    template <typename T>
    Size<T> adjustedSize(Size<T> size, Application::Window::WindowStyle style, bool menu, Application::Window::ExtendedWindowStyle exStyle) {
        RECT r;
        r.left = 0;
        r.right = size.width;
        r.top = 0;
        r.bottom = size.height;
        if (!AdjustWindowRectEx(&r, static_cast<std::underlying_type_t<Application::Window::WindowStyle>>(style),
                                menu, static_cast<std::underlying_type_t<Application::Window::ExtendedWindowStyle>>(exStyle)))
            throw std::runtime_error { "Adjusting window rectangle failed" };

        return { r.right - r.left, r.bottom - r.top };
    }
}