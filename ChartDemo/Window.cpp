#include "Window.h"

#include <stdexcept>

namespace Application::Window {
    using namespace std;
    using namespace Application::Util;


    HWND Window::getHwnd() const noexcept {
        return windowHandle;
    }

    void Window::Register(WNDPROC windowProcedure, HINSTANCE instance, std::wstring_view className, ClassStyle style, int cbClsExtra, int cbWndExtra, std::optional<std::wstring_view> menuName, HICON icon, HCURSOR cursor, HBRUSH background, HICON iconSmall) {
        WNDCLASSEX wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = (unsigned) style;
        wcex.lpfnWndProc = windowProcedure;
        wcex.cbClsExtra = cbClsExtra;
        wcex.cbWndExtra = cbWndExtra;
        wcex.hInstance = instance;
        wcex.hIcon = icon;
        wcex.hCursor = cursor;
        wcex.hbrBackground = background;
        wcex.lpszMenuName = menuName.has_value() ? (LPCWSTR) menuName.value().data() : NULL;
        wcex.lpszClassName = (LPCWSTR) className.data();
        wcex.hIconSm = iconSmall;

        if (!RegisterClassEx(&wcex))
            throw runtime_error { "Register window failed" };
    }

    void Window::setPosition(Point<int> pt) noexcept {
        this->position = pt;
        MoveWindow(this->windowHandle, pt.x, pt.y, this->size.width, size.height, 0);
    }

    void Window::setSize(Size<int> sz) noexcept {
        this->size = sz;
        MoveWindow(this->windowHandle, position.x, position.y, sz.width, sz.height, 0);
    }

    Size<int> Window::getSize() const noexcept {
        return this->size;
    }

    void Window::onParentResize(Size<int> sz) noexcept {

    }

    void Window::onResize(Size<int> sz) noexcept {

    }

    Window::Window(Point<int> pos, Size<int> sz) noexcept : Window { pos, sz, nullptr } {

    }

    Window::Window(Point<int> pos, Size<int> sz, Window* prnt) noexcept : position { pos }, size { sz }, parent { prnt } {

    }

    Window::~Window() noexcept {
        DestroyWindow(windowHandle);
    }

    Window::Window(Window&& wnd) noexcept : parent { wnd.parent }, position { wnd.position }, size { wnd.size }, windowHandle { wnd.windowHandle } {
        wnd.windowHandle = NULL;
    }

    Window& Window::operator=(Window&& wnd) noexcept {
        if (windowHandle) DestroyWindow(windowHandle);
        windowHandle = wnd.windowHandle;
        wnd.windowHandle = NULL;
        parent = wnd.parent;
        position = wnd.position;
        size = wnd.size;

        return *this;
    }

}