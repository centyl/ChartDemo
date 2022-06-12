#include "OpenGlRenderTarget.h"

#include "File.h"
#include "Stats.h"

namespace Application::Rendering {

    using namespace std;
    using namespace Application::OpenGL;
    using namespace Application::Util;

    OpenGlRenderTarget::OpenGlRenderTarget(Window::Window* parent, Size<int> s)
        : RenderTarget { s }, parent { parent } {
        this->deviceContext = GetDC(parent->getHwnd());
        if (!this->deviceContext)
            throw runtime_error { "OpenGL context creation failed" };

        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32,
            0,0,0,0,0,0,
            0,
            0,
            0,
            0,0,0,0,
            24,
            8,
            0,
            PFD_MAIN_PLANE,
            0,
            0,0,0
        };

        auto pixelFormat { ChoosePixelFormat(this->deviceContext, &pfd) };
        if (!pixelFormat)
            throw runtime_error { "OpenGL context creation failed" };

        if (!SetPixelFormat(this->deviceContext, pixelFormat, &pfd))
            throw runtime_error { "OpenGL context creation failed" };

        auto context { wglCreateContext(this->deviceContext) };
        if (!context)
            throw runtime_error { "OpenGL context creation failed" };

        if (!wglMakeCurrent(this->deviceContext, context))
            throw runtime_error { "OpenGL context creation failed" };

        if (glewInit() != GLEW_OK)
            throw runtime_error { "OpenGL context creation failed" };

        glGenBuffers(1, &circleBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, circleBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(circleApproximation), circleApproximation.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &instancePointBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, instancePointBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Point<float>) * 1000, nullptr, GL_DYNAMIC_DRAW);

        glGenBuffers(1, &lineBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Line<float>) * 1000, nullptr, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        auto circleShader { make_unique<Shader>(ShaderType::VertexShader, readFile("circle_shader.vs")) };
        auto lineShader { make_unique<Shader>(ShaderType::VertexShader, readFile("line_shader.vs")) };
        auto colorShader { make_unique<Shader>(ShaderType::FragmentShader, readFile("color_shader.fs")) };

        pointProgram = make_unique<Program>();
        pointProgram->attach(circleShader.get());
        pointProgram->attach(colorShader.get());
        pointProgram->link();
        pointProgram->validate();

        lineProgram = make_unique<Program>();
        lineProgram->attach(lineShader.get());
        lineProgram->attach(colorShader.get());
        lineProgram->link();
        lineProgram->validate();
    }

    OpenGlRenderTarget::~OpenGlRenderTarget() noexcept {
        glDeleteBuffers(1, &circleBuffer);
        glDeleteBuffers(1, &instancePointBuffer);
        glDeleteBuffers(1, &lineBuffer);
    }

    OpenGlRenderTarget::OpenGlRenderTarget(OpenGlRenderTarget&& rt) noexcept : RenderTarget { move(rt) },
        parent { rt.parent }, deviceContext { rt.deviceContext }, pointProgram { move(rt.pointProgram) }, lineProgram { move(rt.lineProgram) },
        circleBuffer { exchange(rt.circleBuffer,0) }, instancePointBuffer { exchange(rt.instancePointBuffer,0) }, lineBuffer { exchange(rt.lineBuffer,0) }
    {

    }

    OpenGlRenderTarget& OpenGlRenderTarget::operator=(OpenGlRenderTarget&& rt) noexcept {
        RenderTarget::operator=(move(rt));

        glDeleteBuffers(1, &circleBuffer);
        glDeleteBuffers(1, &instancePointBuffer);
        glDeleteBuffers(1, &lineBuffer);

        parent = rt.parent;
        deviceContext = rt.deviceContext;
        pointProgram = move(rt.pointProgram);
        lineProgram = move(rt.lineProgram);
        circleBuffer = exchange(rt.circleBuffer, 0);
        instancePointBuffer = exchange(rt.instancePointBuffer, 0);
        lineBuffer = exchange(rt.lineBuffer, 0);

        return *this;
    }

    void OpenGlRenderTarget::beginDraw() const noexcept {
        Stats::begin();
    }

    void OpenGlRenderTarget::endDraw() const noexcept {
        SwapBuffers(this->deviceContext);
        Stats::end();
    }

    void OpenGlRenderTarget::clear(Color c) const noexcept {
        glClearColor(c.r, c.g, c.b, c.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGlRenderTarget::resize(Size<int> s) noexcept {
        this->size = s;
        glViewport(0, 0, s.width, s.height);
    }

    void OpenGlRenderTarget::draw(const vector<Point<float>>& points, float r, Color c) const noexcept {
        auto [width, height] { parent->getSize() };

        pointProgram->use();

        glUniform2f(pointProgram->getUniformLocation("windowSize"), width, height);
        glUniform1f(pointProgram->getUniformLocation("r"), r);
        glUniform4fv(pointProgram->getUniformLocation("color"), 1, (GLfloat*) (&c));

        glBindBuffer(GL_ARRAY_BUFFER, circleBuffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, instancePointBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Point<float>) * points.size(), points.data());
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribDivisor(1, 1);

        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, fragmentCount + 2, points.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGlRenderTarget::draw(Line<float> line, Color color, float thickness) const noexcept {
        auto [width, height] { parent->getSize() };

        glLineWidth(thickness);

        lineProgram->use();

        glUniform2f(lineProgram->getUniformLocation("windowSize"), width, height);
        glUniform4fv(lineProgram->getUniformLocation("color"), 1, (GLfloat*) (&color));

        glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Line<float>), &line);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_LINES, 0, 2);

        glDisableVertexAttribArray(0);
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGlRenderTarget::draw(const vector<Line<float>>& lines, Color c, float thickness) const noexcept {
        auto [width, height] { parent->getSize() };

        glLineWidth(thickness);

        lineProgram->use();

        glUniform2f(lineProgram->getUniformLocation("windowSize"), width, height);
        glUniform4fv(lineProgram->getUniformLocation("color"), 1, (GLfloat*) (&c));

        glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Line<float>) * lines.size(), lines.data());
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_LINES, 0, lines.size() * 2);

        glDisableVertexAttribArray(0);
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGlRenderTarget::drawLineStrip(const vector<Point<float>>& points, Color c, float thickness) const noexcept {
        auto [width, height] { parent->getSize() };

        glLineWidth(thickness);

        lineProgram->use();

        glUniform2f(lineProgram->getUniformLocation("scaling"), 2.0 / width, 2.0 / height);
        glUniform4fv(lineProgram->getUniformLocation("color"), 1, (GLfloat*) (&c));

        glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Point<float>) * points.size(), points.data());
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_LINE_STRIP, 0, points.size());

        glDisableVertexAttribArray(0);
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGlRenderTarget::drawText(const wstring& text, Size<int> size, Point<float> pos) const noexcept {

    }

}