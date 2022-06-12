#include <memory>
#include <d2d1.h>
#include <stdexcept>
#include <cstring>

#include "Application.h"
#include "Window.h"
#include "MainWindow.h"
#include "Canvas.h"
#include "Toolbar.h"

#include "Chart.h"
#include "LineChart.h"
#include "ScatterChart.h"
#include "Constants.h"
#include "Stats.h"

using namespace std;

namespace Application {
    using namespace Window;
    using namespace Rendering;
    using namespace Util;
    using namespace Chart;

    namespace {
        static HINSTANCE instance {};

        static unique_ptr<MainWindow> mainWindow {};

        static ID2D1Factory* factory {};

        static Canvas* cv {};
        static Toolbar* tb {};

        LRESULT CALLBACK ToolbarEventHandler(
            _In_ HWND hWnd,
            _In_ UINT message,
            _In_ WPARAM wParam,
            _In_ LPARAM lParam) {
            switch (message) {
                case WM_COMMAND:
                    tb->onButtonClick();
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }

        LRESULT CALLBACK MainWindowEventHandler(
            _In_ HWND hWnd,
            _In_ UINT message,
            _In_ WPARAM wParam,
            _In_ LPARAM lParam) {
            switch (message) {
                case WM_DESTROY:
                    PostQuitMessage(0);
                    break;
                case WM_SIZING:
                {
                    auto r { *((RECT*) lParam) };
                    mainWindow->onResize({ r.right - r.left,r.bottom - r.top });
                }
                break;

                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }

        LRESULT CALLBACK CanvasEventHandler(
            _In_ HWND hWnd,
            _In_ UINT message,
            _In_ WPARAM wParam,
            _In_ LPARAM lParam) {
            switch (message) {
                case WM_MOUSEMOVE:
                    cv->onMouseMove({ LOWORD(lParam), HIWORD(lParam) });
                    break;
                case WM_MOUSELEAVE:
                    cv->onMouseLeave();
                    break;
                case WM_PAINT:
                    cv->draw();
                    SetWindowText(mainWindow->getHwnd(), Stats::getStats().c_str());
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
    }

    void RegisterWindows() {
        MainWindow::Register(MainWindowEventHandler, instance);
        Toolbar::Register(ToolbarEventHandler, instance);
        Canvas::Register(CanvasEventHandler, instance);
    }

    void CreateWindows() {
        mainWindow = make_unique<MainWindow>(ExtendedWindowStyle::OverlappedWindow, L"Hello",
                                             (WindowStyle) (static_cast<underlying_type_t<WindowStyle>>(WindowStyle::OverlappedWindow | WindowStyle::Visible)),
                                             Point<int> { 0, 0 },
                                             Application::defaultWindowSize,
                                             (HMENU) NULL, instance, (void*) NULL, nullptr);

        auto toolbar { make_unique<Toolbar>(
            ExtendedWindowStyle::LtrReading,
            L"AA",
            (WindowStyle) (static_cast<underlying_type_t<WindowStyle>>(WindowStyle::Child | WindowStyle::Visible)),
            Point<int> { mainWindow->getClientSize().width - Application::toolbarWidth, 0 },
            Size<int>{ Application::toolbarWidth, mainWindow->getClientSize().height },
            mainWindow.get(), (HMENU) NULL, instance, (void*) NULL) };
        tb = toolbar.get();

        auto canvas { make_unique<Canvas>(ExtendedWindowStyle::LtrReading, L"AAA",
            (WindowStyle) (static_cast<underlying_type_t<WindowStyle>>(WindowStyle::Child | WindowStyle::Visible)),
            Point<int> { 0, 0 },
            Size<int> { mainWindow->getClientSize().width - Application::toolbarWidth, mainWindow->getClientSize().height },
            mainWindow.get(), (HMENU) NULL, instance, (void*) NULL) };
        cv = canvas.get();

        canvas->setRenderTargets(
            make_unique<D2DRenderTarget>(canvas->getHwnd(), cv->getSize()),
            make_unique<OpenGlRenderTarget>(canvas.get(), cv->getSize()));

        canvas->setRenderTarget(RenderMethod::Direct2D);

        toolbar->add(make_unique<Button>(
            L"Direct2D",
            (WindowStyle) (static_cast<underlying_type_t<WindowStyle>>(WindowStyle::TabStop | WindowStyle::Visible | WindowStyle::Child | WindowStyle::Group)),
            ButtonStyle::AutoRadioButton,
            Point<int> { 10, 10 },
            Application::toolbarButtonSize,
            tb,
            instance,
            []() -> void { Application::cv->setRenderTarget(RenderMethod::Direct2D); }, true));

        toolbar->add(make_unique<Button>(
            L"OpenGL",
            (WindowStyle) (static_cast<underlying_type_t<WindowStyle>>(WindowStyle::TabStop | WindowStyle::Visible | WindowStyle::Child)),
            ButtonStyle::AutoRadioButton,
            Point<int> { 10, 30 },
            Application::toolbarButtonSize,
            tb,
            instance,
            []()->void {Application::cv->setRenderTarget(RenderMethod::OpenGL); }, false));

        toolbar->add(make_unique<Button>(
            L"Line chart",
            (WindowStyle) (static_cast<underlying_type_t<WindowStyle>>(WindowStyle::TabStop | WindowStyle::Visible | WindowStyle::Child | WindowStyle::Group)),
            ButtonStyle::AutoRadioButton,
            Point<int> { 10, 70 },
            Application::toolbarButtonSize,
            tb,
            instance,
            []() -> void { cv->setChart(make_unique<LineChart>(cv->getChart())); }, true));

        toolbar->add(make_unique<Button>(
            L"Scatter chart",
            (WindowStyle) (static_cast<std::underlying_type_t<WindowStyle>>(WindowStyle::TabStop | WindowStyle::Visible | WindowStyle::Child)),
            ButtonStyle::AutoRadioButton,
            Point<int> { 10, 90 },
            Application::toolbarButtonSize,
            tb,
            instance,
            []() -> void { cv->setChart(make_unique<ScatterChart>(cv->getChart())); }, false));

        toolbar->add(make_unique<Button>(
            L"y = x",
            (WindowStyle) (static_cast<underlying_type_t<WindowStyle>>(WindowStyle::TabStop | WindowStyle::Visible | WindowStyle::Child | WindowStyle::Group)),
            ButtonStyle::AutoRadioButton,
            Point<int> { 10, 130 },
            Application::toolbarButtonSize,
            tb,
            instance,
            []() -> void { cv->getChart()->setFunction([](float f) -> float { return f; }); }, true));

        toolbar->add(make_unique<Button>(
            L"y = x * x",
            (WindowStyle) (static_cast<underlying_type_t<WindowStyle>>(WindowStyle::TabStop | WindowStyle::Visible | WindowStyle::Child)),
            ButtonStyle::AutoRadioButton,
            Point<int> { 10, 150 },
            Application::toolbarButtonSize,
            tb,
            instance,
            []() -> void { cv->getChart()->setFunction([](float f) -> float { return f * f; }); }, false));

        toolbar->add(make_unique<Button>(
            L"y = sin(x / 5)",
            (WindowStyle) (static_cast<underlying_type_t<WindowStyle>>(WindowStyle::TabStop | WindowStyle::Visible | WindowStyle::Child)),
            ButtonStyle::AutoRadioButton,
            Point<int> { 10, 170 },
            Application::toolbarButtonSize,
            tb,
            instance,
            []() -> void { cv->getChart()->setFunction([](float f) -> float { return sin(f / 5); }); }, false));

        toolbar->add(make_unique<Button>(
            L"y = x * sin(x)",
            (WindowStyle) (static_cast<underlying_type_t<WindowStyle>>(WindowStyle::TabStop | WindowStyle::Visible | WindowStyle::Child)),
            ButtonStyle::AutoRadioButton,
            Point<int> { 10, 190 },
            Application::toolbarButtonSize,
            tb,
            instance,
            []() -> void { cv->getChart()->setFunction([](float f) -> float { return sin(f) * f; }); }, false));

        toolbar->add(make_unique<Button>(
            L"samples: 20",
            (WindowStyle) (static_cast<std::underlying_type_t<WindowStyle>>(WindowStyle::TabStop | WindowStyle::Visible | WindowStyle::Child | WindowStyle::Group)),
            ButtonStyle::AutoRadioButton,
            Point<int> { 10, 230 },
            Application::toolbarButtonSize,
            tb,
            instance,
            []() -> void { cv->getChart()->setSamples(20); }, false));

        toolbar->add(make_unique<Button>(
            L"samples: 100",
            (WindowStyle) (static_cast<std::underlying_type_t<WindowStyle>>(WindowStyle::TabStop | WindowStyle::Visible | WindowStyle::Child)),
            ButtonStyle::AutoRadioButton,
            Point<int> { 10, 250 },
            Application::toolbarButtonSize,
            tb,
            instance,
            []() -> void { cv->getChart()->setSamples(100); }, false));


        toolbar->add(make_unique<Button>(
            L"samples: 200",
            (WindowStyle) (static_cast<underlying_type_t<WindowStyle>>(WindowStyle::TabStop | WindowStyle::Visible | WindowStyle::Child)),
            ButtonStyle::AutoRadioButton,
            Point<int> { 10, 270 },
            Application::toolbarButtonSize,
            tb,
            instance,
            []() -> void { cv->getChart()->setSamples(200); }, false));

        toolbar->add(make_unique<Button>(
            L"samples: 1000",
            (WindowStyle) (static_cast<std::underlying_type_t<WindowStyle>>(WindowStyle::TabStop | WindowStyle::Visible | WindowStyle::Child)),
            ButtonStyle::AutoRadioButton,
            Point<int> { 10, 290 },
            Application::toolbarButtonSize,
            tb,
            instance,
            []() -> void { cv->getChart()->setSamples(1000); }, true));

        toolbar->add(make_unique<Button>(
            L"<-100;100>",
            (WindowStyle) (static_cast<underlying_type_t<WindowStyle>>(WindowStyle::TabStop | WindowStyle::Visible | WindowStyle::Child | WindowStyle::Group)),
            ButtonStyle::AutoRadioButton,
            Point<int> { 10, 330 },
            Application::toolbarButtonSize,
            tb,
            instance,
            []() -> void { cv->getChart()->setRange({ -100,100 }); }, true));

        toolbar->add(make_unique<Button>(
            L"<-10;10>",
            (WindowStyle) (static_cast<underlying_type_t<WindowStyle>>(WindowStyle::TabStop | WindowStyle::Visible | WindowStyle::Child)),
            ButtonStyle::AutoRadioButton,
            Point<int> { 10, 350 },
            Application::toolbarButtonSize,
            tb,
            instance,
            []() -> void { cv->getChart()->setRange({ -10,10 }); }, false));

        toolbar->add(make_unique<Button>(
            L"<0;1>",
            (WindowStyle) (static_cast<std::underlying_type_t<WindowStyle>>(WindowStyle::TabStop | WindowStyle::Visible | WindowStyle::Child)),
            ButtonStyle::AutoRadioButton,
            Point<int> { 10, 370 },
            Application::toolbarButtonSize,
            tb,
            instance,
            []() -> void { cv->getChart()->setRange({ 0,1 }); }, false));

        toolbar->add(make_unique<Button>(
            L"<0;500>",
            (WindowStyle) (static_cast<std::underlying_type_t<WindowStyle>>(WindowStyle::TabStop | WindowStyle::Visible | WindowStyle::Child)),
            ButtonStyle::AutoRadioButton,
            Point<int> { 10, 390 },
            Application::toolbarButtonSize,
            tb,
            instance,
            []() -> void { cv->getChart()->setRange({ 0, 500 }); }, false));

        mainWindow->add(move(toolbar));
        mainWindow->add(move(canvas));

        ShowWindow(mainWindow->getHwnd(), 10);
        ShowScrollBar(mainWindow->getHwnd(), SB_BOTH, 0);
    }

    void initialize(HINSTANCE instance) {
        Application::instance = instance;

        RegisterWindows();

        D2DRenderTarget::Initialize();

        CreateWindows();

        cv->setChart(make_unique<LineChart>(
            Size<int> { mainWindow->getClientSize().width - 150, mainWindow->getClientSize().height },
            pair<float, float> { -100, 100 },
            1000,
            [](float x) -> float { return x; }));

    }

    int messageLoop() noexcept {
        MSG msg;
        auto res { 0 };
        while (res = GetMessage(&msg, NULL, 0, 0)) {
            if (res == -1)
                return -1;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return (int) msg.wParam;
    }

}