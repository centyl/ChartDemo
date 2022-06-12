#pragma once

#include <functional>

#include "Window.h"
#include "ButtonStyle.h"

namespace Application::Window {

    class Button : public Window {
        public:

        Button(std::wstring text, Application::Window::WindowStyle ws, Application::Window::ButtonStyle bs,
               Application::Util::Point<int> pos, Application::Util::Size<int> sz, Window* prnt, HINSTANCE ic,
               std::function<void()> onClick, bool active
        );

        void click() const noexcept;

        void setState(bool b) noexcept;

        bool getState() const noexcept;

        private:
        bool isChecked { false };
        std::function<void()> onClick { []() ->void { return; } };
    };

}

