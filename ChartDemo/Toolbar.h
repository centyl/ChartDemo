#pragma once

#include "Window.h"
#include "Button.h"

namespace Application::Window {

    class Toolbar : public Window {
        public:

        static void Register(WNDPROC windowProcedure, HINSTANCE instance);

        void onButtonClick() const noexcept;

        void add(std::unique_ptr<Application::Window::Button> wnd) noexcept;

        virtual void onParentResize(Application::Util::Size<int> sz) noexcept override;

        Toolbar(Application::Window::ExtendedWindowStyle dwExStyle,
                std::wstring&& windowName,
                Application::Window::WindowStyle dwStyle,
                Application::Util::Point<int> position,
                Application::Util::Size<int> size,
                Window* prnt, HMENU hMenu, HINSTANCE instance, void* lpParam
        );

        private:
        inline const static std::wstring className { L"Toolbar" };

        std::wstring windowTitle {};
        std::vector<std::unique_ptr<Button>> children {};

    };

}