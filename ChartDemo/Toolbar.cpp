#include "Toolbar.h"

#include <algorithm>
#include <stdexcept>

namespace Application::Window {

    using namespace std;
    using namespace Application::Util;

    void Toolbar::Register(WNDPROC windowProcedure, HINSTANCE instance) {
        Window::Register(windowProcedure, instance, className, ClassStyle::HorizontalRedraw | ClassStyle::VerticalRedraw, 0, 0,
                         std::nullopt, LoadIcon(instance, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
                         (HBRUSH) (COLOR_GRAYTEXT + 1), LoadIcon(instance, IDI_APPLICATION));
    }

    Toolbar::Toolbar(ExtendedWindowStyle dwExStyle, std::wstring&& windowName, WindowStyle dwStyle
                     , Point<int> position, Size<int> size, Window* prnt, HMENU hMenu, HINSTANCE instance, void* lpParam)
        : Window { position, size, prnt }, windowTitle { windowName } {

        windowHandle = CreateWindowEx(
            (DWORD) dwExStyle,
            (LPCWSTR) className.c_str(),
            (LPCWSTR) windowTitle.c_str(),
            (unsigned) dwStyle,
            position.x, position.y,
            size.width, size.height,
            prnt->getHwnd(),
            hMenu,
            instance,
            lpParam
        );

        if (windowHandle == NULL)
            throw runtime_error { "Creating toolbar window failed" };
    }

    void Toolbar::onButtonClick() const noexcept {
        for (auto&& c : children) {
            auto currentState { c->getState() };
            auto nextState { SendMessage(c->getHwnd(), BM_GETCHECK, 0, 0) == BST_CHECKED };
            if (nextState && !currentState)
                c->click();

            c->setState(nextState);
        };
    }

    void Toolbar::add(unique_ptr<Button> wnd) noexcept {
        children.push_back(move(wnd));
    }

    void Toolbar::onParentResize(Size<int> sz) noexcept {
        setPosition({ sz.width - 150,0 });
        setSize({ 150,sz.height });
    }

}