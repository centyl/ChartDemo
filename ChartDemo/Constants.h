#pragma once

#include "Color.h"
#include "Size.h"

namespace Application {

    /*
    Values pertaining to colors or appearance of a chart could instead be stored in a Chart object,
    with ability for a user to modify them via UI. However it would require creating better UI,
    possibly using other libraries, which was not a purpose of this simple demo.
    */

    static const Application::Util::Color labelColor { 0.3f, 0.0f, 0.8f };
    static const int labelFontSize { 8 };
    static const std::wstring labelFontFamily { L"Verdana" };
    static const Application::Util::Size<int> labelSize { 30,10 };

    static const Application::Util::Color chartAxisColor { 0.2f,0.2f,0.4f };
    static const float chartLineThickness { 0.4f };

    static const int chartMarginTop { 10 };
    static const int chartMarginBottom { 30 };
    static const int chartMarginLeft { 10 };
    static const int chartMarginRight { 30 };

    static const Application::Util::Color chartFunctionColor { 0.9f,0.2f,0.2f };
    static const float chartFunctionThickness { 0.5f };

    static const Application::Util::Size<int> defaultWindowSize { 600,450 };
    static const int toolbarWidth { 150 };
    static const Application::Util::Size<int> toolbarButtonSize { 130,20 };

}