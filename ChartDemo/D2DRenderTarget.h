#pragma once

#include "RenderTarget.h"
#include "Size.h"

#include <d2d1.h>
#include <dwrite.h>

namespace Application::Rendering {

    class D2DRenderTarget : public RenderTarget {
        public:
        static void Initialize();

        void beginDraw() const noexcept override;
        void endDraw() const noexcept override;

        void clear(Application::Util::Color c) const noexcept override;
        void resize(Application::Util::Size<int> s) noexcept override;

        void drawText(const std::wstring& text, Application::Util::Size<int> size, Application::Util::Point<float> pos)  const noexcept override;
        void draw(Application::Util::Line<float> line, Application::Util::Color color, float thickness) const noexcept override;
        void draw(const std::vector<Application::Util::Point<float>>& points, float r, Application::Util::Color c) const noexcept override;
        void draw(const std::vector<Application::Util::Line<float>>& lines, Application::Util::Color c, float thickness) const noexcept override;
        void drawLineStrip(const std::vector<Application::Util::Point<float>>& points, Application::Util::Color c, float thickness) const noexcept override;

        D2DRenderTarget(HWND windowHandle, Application::Util::Size<int> size);

        ~D2DRenderTarget() noexcept;
        D2DRenderTarget(const D2DRenderTarget& rt);
        D2DRenderTarget& operator=(const D2DRenderTarget& rt);
        D2DRenderTarget(D2DRenderTarget&& rt) noexcept;
        D2DRenderTarget& operator=(D2DRenderTarget&& rt) noexcept;

        private:
        ID2D1HwndRenderTarget* renderTarget { nullptr };
        IDWriteTextFormat* textFormat { nullptr };
    };

}
