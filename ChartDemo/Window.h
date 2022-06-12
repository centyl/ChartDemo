#pragma once

#include <Windows.h>
#include <string_view>
#include <optional>

#include "ClassStyle.h"
#include "Size.h"
#include "Point.h"

namespace Application::Window {

    class Window {
        public:

        static void Register(WNDPROC windowProcedure, HINSTANCE instance,
                             std::wstring_view className, Application::Window::ClassStyle style,
                             int cbClsExtra, int cbWndExtra,
                             std::optional<std::wstring_view> menuName,
                             HICON icon, HCURSOR cursor, HBRUSH background, HICON iconSmall);

        Application::Util::Size<int> getSize() const noexcept;
        HWND getHwnd() const noexcept;

        void setPosition(Application::Util::Point<int> pt) noexcept;
        void setSize(Application::Util::Size<int> sz) noexcept;

        virtual void onParentResize(Application::Util::Size<int> sz) noexcept;
        virtual void onResize(Application::Util::Size<int> sz) noexcept;

        Window(Application::Util::Point<int> pos, Application::Util::Size<int> sz) noexcept;
        Window(Application::Util::Point<int> pos, Application::Util::Size<int> sz, Window* prnt) noexcept;

        virtual ~Window() noexcept;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&& wnd) noexcept;
        Window& operator=(Window&& wnd) noexcept;

        protected:

        Window* parent { nullptr };
        Application::Util::Point<int> position { 0,0 };
        Application::Util::Size<int> size { 0,0 };
        HWND windowHandle { NULL };

    };

}