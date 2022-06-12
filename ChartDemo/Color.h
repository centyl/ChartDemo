#pragma once

namespace Application::Util {

    struct Color {
        float r;
        float g;
        float b;
        float a;

        Color(float r, float g, float b, float a = 1.0f) noexcept;

        Color(int r, int g, int b, int a = 255) noexcept;

    };

}