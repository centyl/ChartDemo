#pragma once

#include "RenderTarget.h"

namespace Application::Rendering {

    class Drawable {
        public:

        virtual void draw(const RenderTarget* rt) const noexcept = 0;

        Drawable() noexcept = default;
        virtual ~Drawable() noexcept = default;
        Drawable(const Drawable&) = delete;
        Drawable& operator=(const Drawable&) = delete;
        Drawable(Drawable&&) = delete;
        Drawable& operator=(Drawable&&) = delete;

    };

}