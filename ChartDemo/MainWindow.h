#pragma once

#include "Window.h"

#include <memory>
#include <vector>

namespace Application::Window {

    class MainWindow : public Window {
        public:
        static void Register(WNDPROC windowProcedure, HINSTANCE instance);

        Application::Util::Size<int> getClientSize() const noexcept;

        void add(std::unique_ptr<Window> wnd) noexcept;

        virtual void onResize(Application::Util::Size<int> sz) noexcept override;

        MainWindow(ExtendedWindowStyle dwExStyle,
                   std::wstring&& windowName,
                   WindowStyle dwStyle,
                   Application::Util::Point<int> position, Application::Util::Size<int> clientAreaSize,
                   HMENU hMenu, HINSTANCE instance, void* lpParam, Window* prnt);

        private:

        inline const static std::wstring className { L"DesktopApp" };
        std::wstring windowTitle {};

        Application::Util::Size<int> clientSize { 0,0 };

        std::vector<std::unique_ptr<Window>> children {};

    };

}