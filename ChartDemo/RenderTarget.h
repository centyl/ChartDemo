#pragma once

#include <vector>
#include <string>

#include "Color.h"
#include "Line.h"
#include "Size.h"

namespace Application::Rendering {

    class RenderTarget {
        public:
        Application::Util::Size<int> getSize() noexcept;

        virtual void beginDraw() const noexcept = 0;

        virtual void endDraw() const noexcept = 0;

        virtual void clear(Application::Util::Color c) const noexcept = 0;

        virtual void resize(Application::Util::Size<int> s) noexcept = 0;

        virtual void draw(Application::Util::Line<float> line, Application::Util::Color color, float thickness) const noexcept = 0;
        virtual void draw(const std::vector<Application::Util::Point<float>>& points, float r, Application::Util::Color c) const noexcept = 0;
        virtual void draw(const std::vector<Application::Util::Line<float>>& lines, Application::Util::Color c, float thickness) const noexcept = 0;
        virtual void drawLineStrip(const std::vector<Application::Util::Point<float>>& points, Application::Util::Color c, float thickness) const noexcept = 0;
        virtual void drawText(const std::wstring& text, Application::Util::Size<int> size, Application::Util::Point<float> pos) const noexcept = 0;

        virtual ~RenderTarget() noexcept = default;

        protected:

        RenderTarget(Application::Util::Size<int> size) noexcept;

        RenderTarget(const RenderTarget&) = default;
        RenderTarget& operator=(const RenderTarget&) = default;
        RenderTarget(RenderTarget&&) = default;
        RenderTarget& operator=(RenderTarget&&) = default;

        Application::Util::Size<int> size { 0,0 };
    };

}