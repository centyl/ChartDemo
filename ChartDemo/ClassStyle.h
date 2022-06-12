#pragma once

#include <Windows.h>

namespace Application::Window {

    enum class ClassStyle {
        VerticalRedraw = CS_VREDRAW,
        HorizontalRedraw = CS_HREDRAW,
        DoubleClicks = CS_DBLCLKS,
        OwnDeviceContext = CS_OWNDC,
        ClassDeviceContext = CS_CLASSDC,
        ParentDeviceContext = CS_PARENTDC,
        NoClose = CS_NOCLOSE,
        SaveBits = CS_SAVEBITS,
        ByteAlignClient = CS_BYTEALIGNCLIENT,
        ByteAlignWindow = CS_BYTEALIGNWINDOW,
        GlobalClass = CS_GLOBALCLASS,
        IME = CS_IME,
        DropShadow = CS_DROPSHADOW
    };

    ClassStyle& operator|(ClassStyle lhs, const ClassStyle& rhs);
}