#include "Canvas.h"

#include <stdexcept>

namespace Application::Window {

    using namespace std;
    using namespace Application::Rendering;
    using namespace Application::Util;

    void Canvas::Register(WNDPROC windowProcedure, HINSTANCE instance) {
        Window::Register(windowProcedure, instance, className,
                         ClassStyle::HorizontalRedraw | ClassStyle::VerticalRedraw | ClassStyle::OwnDeviceContext, 0, 0,
                         std::nullopt, LoadIcon(instance, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
                         (HBRUSH) (COLOR_WINDOW + 1), LoadIcon(instance, IDI_APPLICATION));

    }

    void Canvas::draw() const noexcept {
        renderTarget->beginDraw();

        renderTarget->clear({ 1.0f,1.0f,1.0f });

        chart->draw(renderTarget);

        if (mousePoint.has_value()) {
            renderTarget->draw(Line<float> { { (float) mousePoint.value().x, 0 }, { (float) mousePoint.value().x,(float) size.height }}, { 0.0f,1.0f,0.2f,1.0f }, 0.3f);
            renderTarget->draw(Line<float> { { 0, (float) mousePoint.value().y }, { (float) size.width,(float) mousePoint.value().y }}, { 0.0f,1.0f,0.2f,1.0f }, 0.3f);
        }

        renderTarget->endDraw();
    }

    void Canvas::setChart(unique_ptr<Application::Chart::Chart> chrt) noexcept {
        this->chart.swap(chrt);
    }

    Application::Chart::Chart* Canvas::getChart() const noexcept {
        return chart.get();
    }

    Canvas::Canvas(ExtendedWindowStyle dwExStyle, std::wstring&& windowName, WindowStyle dwStyle, Point<int> position, Size<int> size,
                   Window* parent, HMENU hMenu, HINSTANCE instance,
                   void* lpParam) : Window { position, size, parent }, windowTitle { windowName } {

        windowHandle = CreateWindowEx(
            (DWORD) dwExStyle,
            (LPCWSTR) className.c_str(),
            (LPCWSTR) windowTitle.c_str(),
            (unsigned) dwStyle,
            position.x, position.y,
            size.width, size.height,
            parent->getHwnd(),
            hMenu,
            instance,
            lpParam
        );
        if (windowHandle == NULL)
            throw runtime_error { "Creating canvas window failed" };

    }

    void Canvas::setSize(Size<int> sz) {
        Window::setSize(sz);
        renderTarget->resize({ sz.width,sz.height });
        chart->setSize(sz);
    }

    void Canvas::setRenderTarget(RenderMethod rm) {
        if (rm == RenderMethod::Direct2D)
            renderTarget = d2dRenderTarget.get();
        else if (rm == RenderMethod::OpenGL)
            renderTarget = openGlRenderTarget.get();

        renderTarget->resize(this->size);
    }

    void Canvas::setRenderTargets(unique_ptr<D2DRenderTarget> d2dRt, unique_ptr<OpenGlRenderTarget> oglRt) {
        this->d2dRenderTarget = std::move(d2dRt);
        this->openGlRenderTarget = std::move(oglRt);
    }

    void Canvas::onMouseMove(Point<unsigned> pos) {
        mousePoint = pos;

        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = windowHandle;
        tme.dwHoverTime = HOVER_DEFAULT;
        TrackMouseEvent(&tme);
    }

    void Canvas::onMouseLeave() {
        mousePoint = std::nullopt;
    }

    void Canvas::onParentResize(Size<int> sz) noexcept {
        setSize({ sz.width - 150, sz.height });
    }

}