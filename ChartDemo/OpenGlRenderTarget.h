#pragma once

#include "RenderTarget.h"

#include <array>
#include <memory>

#include "Program.h"
#include "Window.h"

namespace Application::Rendering {

    // generates array of points belonging to a circle centered at (0, 0) with r=1, to be used for drawing in GL_TRIANGLE_FAN mode
    template <int N> constexpr std::array<Application::Util::Point<float>, N + 2> generateCircleApproximation() noexcept {
        auto fragments = N;
        auto angle = 2 * acos(-1) / fragments;
        auto result = std::array<Application::Util::Point<float>, N + 2>{};
        result[0] = { 0,0 };
        for (auto i = 1; i <= fragments; i++) {
            result[i] = { (float) cos(angle * i), (float) sin(angle * i) };
        }
        result[fragments + 1] = result[1];
        return result;
    }

    class OpenGlRenderTarget : public RenderTarget {
        public:

        void beginDraw() const noexcept override;
        void endDraw() const noexcept override;

        void clear(Application::Util::Color c) const noexcept override;
        void resize(Application::Util::Size<int> s) noexcept override;

        void drawText(const std::wstring& text, Application::Util::Size<int> size, Application::Util::Point<float> pos) const noexcept override;
        void draw(Application::Util::Line<float> line, Application::Util::Color color, float thickness)  const noexcept override;
        void draw(const std::vector<Application::Util::Point<float>>& points, float r, Application::Util::Color c) const noexcept override;
        void draw(const std::vector<Application::Util::Line<float>>& lines, Application::Util::Color c, float thickness) const noexcept override;
        void drawLineStrip(const std::vector<Application::Util::Point<float>>& points, Application::Util::Color c, float thickness) const noexcept override;

        OpenGlRenderTarget(Application::Window::Window* parent, Application::Util::Size<int> s);

        ~OpenGlRenderTarget() noexcept;
        OpenGlRenderTarget(const OpenGlRenderTarget& rt) = delete;
        OpenGlRenderTarget& operator=(const OpenGlRenderTarget& rt) = delete;
        OpenGlRenderTarget(OpenGlRenderTarget&& rt) noexcept;
        OpenGlRenderTarget& operator=(OpenGlRenderTarget&& rt) noexcept;

        private:

        static const int fragmentCount { 8 };
        const std::array<Application::Util::Point<float>, fragmentCount + 2> circleApproximation { generateCircleApproximation<fragmentCount>() };

        Application::Window::Window* parent { nullptr };

        HDC deviceContext { NULL };

        std::unique_ptr<Application::OpenGL::Program> pointProgram {};
        std::unique_ptr<Application::OpenGL::Program> lineProgram {};

        GLuint circleBuffer {};
        GLuint instancePointBuffer {};
        GLuint lineBuffer {};
    };

}