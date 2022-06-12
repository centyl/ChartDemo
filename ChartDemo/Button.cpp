#include "Button.h"

#include <Windows.h>
#include <stdexcept>

namespace Application::Window {

    using namespace std;
    using namespace Application::Util;

    Button::Button(std::wstring text, WindowStyle ws, ButtonStyle bs, Point<int> pos,
                   Size<int> sz, Window* prnt, HINSTANCE ic, function<void()> onClick, bool active)
        : Window { pos,sz }, onClick { onClick }, isChecked { active } {

        this->windowHandle = CreateWindow(
            L"BUTTON",
            text.c_str(),
            ((unsigned long long) ws) | ((unsigned long long)bs),
            pos.x,
            pos.y,
            sz.width,
            sz.height,
            prnt->getHwnd(),
            NULL,
            ic,
            NULL
        );

        if (!this->windowHandle)
            throw runtime_error { "Creating button failed" };

        if (active)
            SendMessage(this->windowHandle, BM_SETCHECK, BST_CHECKED, 0);

    }

    void Button::click() const noexcept {
        onClick();
    }

    void Button::setState(bool b) noexcept {
        isChecked = b;
    }

    bool Button::getState() const noexcept {
        return isChecked;
    }

}