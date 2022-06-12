#pragma once

#include <Windows.h>

#include "Window.h"
#include "Chart.h"
#include "D2DRenderTarget.h"
#include "OpenGlRenderTarget.h"
//
//class Application::Rendering::OpenGlRenderTarget;

namespace Application::Window {
    enum class RenderMethod {
        Direct2D,
        OpenGL
    };

    class Canvas : public Window {
        public:

        static void Register(WNDPROC windowProcedure, HINSTANCE instance);

        Application::Chart::Chart* getChart() const noexcept;

        void draw() const noexcept;

        void setChart(std::unique_ptr<Application::Chart::Chart> chrt) noexcept;

        Canvas(Application::Window::ExtendedWindowStyle dwExStyle,
               std::wstring&& windowName,
               Application::Window::WindowStyle dwStyle,
               Application::Util::Point<int> position, Application::Util::Size<int> size,
               Window* parent, HMENU hMenu, HINSTANCE instance, void* lpParam
        );

        void setSize(Application::Util::Size<int> sz);

        void setRenderTarget(RenderMethod rm);

        void setRenderTargets(std::unique_ptr<Application::Rendering::D2DRenderTarget> d2dRt,
                              std::unique_ptr<Application::Rendering::OpenGlRenderTarget> oglRt);

        void onMouseMove(Application::Util::Point<unsigned> pos);

        void onMouseLeave();

        virtual void onParentResize(Application::Util::Size<int> sz) noexcept override;

        private:

        inline const static std::wstring className = L"Canvas";
        std::wstring windowTitle {};

        Application::Rendering::RenderTarget* renderTarget { nullptr };
        std::unique_ptr<Application::Rendering::D2DRenderTarget> d2dRenderTarget {};
        std::unique_ptr<Application::Rendering::OpenGlRenderTarget> openGlRenderTarget {};

        std::unique_ptr<Application::Chart::Chart> chart {};

        std::optional<Application::Util::Point<unsigned>> mousePoint = std::nullopt;

    };

}