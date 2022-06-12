#include "D2DRenderTarget.h"

#include <algorithm>
#include <stdexcept>

#include "Stats.h"
#include "Constants.h"

namespace Application::Rendering {

    using namespace std;
    using namespace Application::Util;

    namespace {
        ID2D1Factory* factory {};
        IDWriteFactory* writeFactory {};
    }

    void D2DRenderTarget::Initialize() {
        if (D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory) != S_OK)
            throw runtime_error { "D2D1 create factory failed" };

        if (DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory)) != S_OK)
            throw runtime_error { "DirectWrite create factory failed" };
    }

    D2DRenderTarget::D2DRenderTarget(HWND windowHandle, Size<int> size) : RenderTarget { size } {
        if (factory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(
                windowHandle, { (unsigned) size.width, (unsigned) size.height }, D2D1_PRESENT_OPTIONS_IMMEDIATELY
            ), &this->renderTarget) != S_OK)
            throw runtime_error { "D2D create render target failed" };

        if (writeFactory->CreateTextFormat(Application::labelFontFamily.c_str(), NULL,
                                           DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
                                           DWRITE_FONT_STRETCH_NORMAL, Application::labelFontSize, L"", &textFormat) != S_OK)
            throw runtime_error { "DirectWrite create text format failed" };

        textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }

    D2DRenderTarget::~D2DRenderTarget() noexcept {
        renderTarget->Release();
        textFormat->Release();
    }

    D2DRenderTarget::D2DRenderTarget(const D2DRenderTarget& rt) : RenderTarget { rt } {
        this->renderTarget = rt.renderTarget;
        this->renderTarget->AddRef();
        this->textFormat = rt.textFormat;
        this->textFormat->AddRef();
    }

    D2DRenderTarget& D2DRenderTarget::operator=(const D2DRenderTarget& rt) {
        RenderTarget::operator=(rt);
        if (renderTarget) renderTarget->Release();
        renderTarget = rt.renderTarget;
        renderTarget->AddRef();

        if (textFormat) textFormat->Release();
        textFormat = rt.textFormat;
        textFormat->AddRef();

        return *this;
    }

    D2DRenderTarget::D2DRenderTarget(D2DRenderTarget&& rt) noexcept : RenderTarget { move(rt) },
        renderTarget { exchange(rt.renderTarget, nullptr) }, textFormat { exchange(rt.textFormat, nullptr) } {

    }

    D2DRenderTarget& D2DRenderTarget::operator=(D2DRenderTarget&& rt) noexcept {
        RenderTarget::operator=(move(rt));
        if (renderTarget) renderTarget->Release();
        renderTarget = exchange(rt.renderTarget, nullptr);

        if (textFormat) textFormat->Release();
        textFormat = exchange(rt.textFormat, nullptr);

        return *this;
    }

    void D2DRenderTarget::beginDraw() const noexcept {
        Stats::begin();
        renderTarget->BeginDraw();
    }

    void D2DRenderTarget::endDraw() const noexcept {
        renderTarget->EndDraw();
        Stats::end();
    }

    void D2DRenderTarget::clear(Color c) const noexcept {
        renderTarget->Clear({ c.r,c.g,c.b,c.a });
    }

    void D2DRenderTarget::resize(Size<int> s) noexcept {
        this->size = s;
        renderTarget->Resize({ (unsigned) s.width, (unsigned) s.height });
    }

    void D2DRenderTarget::draw(const vector<Point<float>>& points, float r, Color c) const noexcept {
        ID2D1SolidColorBrush* br;
        if (renderTarget->CreateSolidColorBrush({ c.r,c.g,c.b,c.a }, &br) != S_OK)
            return;

        for (auto&& p : points)
            renderTarget->FillEllipse(D2D1::Ellipse({ p.x, p.y }, r, r), br);

        br->Release();
    }

    void D2DRenderTarget::draw(Line<float> line, Color color, float thickness) const noexcept {
        ID2D1SolidColorBrush* brush;
        if (renderTarget->CreateSolidColorBrush({ color.r,color.g,color.b,color.a }, &brush) != S_OK)
            return;

        renderTarget->DrawLine({ line.firstPoint.x,line.firstPoint.y },
                               { line.secondPoint.x,line.secondPoint.y }, brush, thickness);
        brush->Release();
    }

    void D2DRenderTarget::draw(const vector<Line<float>>& lines, Color c, float thickness) const noexcept {
        ID2D1SolidColorBrush* brush;
        if (renderTarget->CreateSolidColorBrush({ c.r,c.g,c.b,c.a }, &brush) != S_OK)
            return;

        for (auto&& l : lines)
            renderTarget->DrawLine({ l.firstPoint.x, l.firstPoint.y }, { l.secondPoint.x, l.secondPoint.y }, brush, thickness);

        brush->Release();
    }

    void D2DRenderTarget::drawLineStrip(const vector<Point<float>>& points, Color c, float thickness) const noexcept {
        ID2D1SolidColorBrush* brush;
        if (renderTarget->CreateSolidColorBrush({ c.r,c.g,c.b,c.a }, &brush) != S_OK)
            return;

        if (points.size() < 2)
            return;

        auto e = prev(cend(points));
        for (auto it = cbegin(points); it != e; it++) {
            renderTarget->DrawLine({ it->x, it->y }, { (it + 1)->x, (it + 1)->y }, brush, thickness);
        }

        brush->Release();
    }

    void D2DRenderTarget::drawText(const wstring& text, Size<int> size, Point<float> pos) const noexcept {
        ID2D1SolidColorBrush* brush;
        if (renderTarget->CreateSolidColorBrush(
            { Application::labelColor.r,
            Application::labelColor.g,
            Application::labelColor.b,
            Application::labelColor.a }, &brush) != S_OK)
            return;

        renderTarget->DrawTextW(text.c_str(), text.length(), textFormat,
                                { pos.x - size.width / 2, pos.y, pos.x + size.width / 2, pos.y + size.height },
                                brush);

        brush->Release();
    }

}