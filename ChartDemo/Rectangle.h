#pragma once

#include "Size.h"
#include "Point.h"

namespace Application::Util {

    template <typename T>
    class Rectangle {
        public:

        Rectangle(Application::Util::Size<T> s) noexcept : Rectangle { { 0,0 }, s } {

        }

        Rectangle(Application::Util::Point<T> topLeft, Application::Util::Size<T> s) noexcept : topLeft { topLeft }, size { s } {

        }

        Rectangle(T left, T right, T top, T bottom) noexcept : Rectangle { { left,top }, { right - left,bottom - top } } {

        }

        Application::Util::Size<T> getSize() const noexcept {
            return size;
        }

        Application::Util::Point<T> getPosition() const noexcept {
            return topLeft;
        }

        void setSize(Application::Util::Size<T> s) noexcept {
            size = s;
        }

        void setPosition(Application::Util::Point<T> p) noexcept {
            topLeft = p;
        }

        Rectangle<T> box(int marginTop, int marginBottom, int marginLeft, int marginRight) const noexcept {
            return Rectangle { { this->topLeft.x + marginLeft, this->topLeft.y + marginTop },
                             { this->size.width - marginLeft - marginRight, this->size.height - marginTop - marginBottom } };
        }

        T left() const noexcept {
            return topLeft.x;
        }

        T right() const noexcept {
            return topLeft.x + size.width;
        }

        T top() const noexcept {
            return topLeft.y;
        }

        T bottom() const noexcept {
            return topLeft.y + size.height;
        }

        T width() const noexcept {
            return size.width;
        }

        T height() const noexcept {
            return size.height;
        }

        private:
        Application::Util::Point<T> topLeft { 0,0 };
        Application::Util::Size<T> size { 0,0 };

    };

}
