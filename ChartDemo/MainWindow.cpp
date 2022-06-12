#include "MainWindow.h"

#include <stdexcept>
#include <algorithm>

namespace Application::Window {

    using namespace std;
    using namespace Application::Util;

    void MainWindow::Register(WNDPROC windowProcedure, HINSTANCE instance) {
        Window::Register(windowProcedure, instance, className, ClassStyle::HorizontalRedraw | ClassStyle::VerticalRedraw, 0, 0,
                         std::nullopt, LoadIcon(instance, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
                         (HBRUSH) (COLOR_WINDOW + 1), LoadIcon(instance, IDI_APPLICATION));

    }

    MainWindow::MainWindow(ExtendedWindowStyle dwExStyle, std::wstring&& windowName, WindowStyle dwStyle,
                           Point<int> position, Size<int> size,
                           HMENU hMenu, HINSTANCE instance, void* lpParam, Window* prnt)
        : Window { position, adjustedSize(size, dwStyle, 0, dwExStyle), prnt }, windowTitle { windowName }, clientSize { size } {
        windowHandle = CreateWindowEx(
            (DWORD) dwExStyle,
            (LPCWSTR) className.c_str(),
            (LPCWSTR) windowTitle.c_str(),
            (DWORD) dwStyle,
            position.x, position.y,
            this->size.width, this->size.height,
            prnt ? prnt->getHwnd() : NULL,
            hMenu,
            instance,
            lpParam
        );

        if (windowHandle == NULL)
            throw runtime_error { "Creating main window failed" };
    }

    Size<int> MainWindow::getClientSize() const noexcept {
        return clientSize;
    }

    void MainWindow::add(unique_ptr<Window> wnd) noexcept {
        children.push_back(move(wnd));
    }

    void MainWindow::onResize(Size<int> sz) noexcept {
        this->size = sz;
        RECT rc;
        GetClientRect(windowHandle, &rc);
        this->clientSize = { rc.right - rc.left,rc.bottom - rc.top };

        for (auto&& c : children)
            c->onParentResize(this->clientSize);
    }
}