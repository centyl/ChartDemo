#include "Chart.h"

#include <algorithm>
#include <iterator>

#include "Rectangle.h"
#include "Constants.h"

namespace Application::Chart {

    using namespace std;
    using namespace Application::Util;
    using namespace Application::Rendering;

    void Chart::setSize(Size<int> sz) noexcept {
        this->size = sz;
        this->calculatePoints();
    }

    void Chart::setFunction(function<float(float)> fnc) noexcept {
        this->f = fnc;
        this->calculatePoints();
    }

    void Chart::setRange(pair<float, float> rng) noexcept {
        this->range = rng;
        this->calculatePoints();
    }

    void Chart::setSamples(int smpl) noexcept {
        this->samples = smpl;
        this->calculatePoints();
    }

    Chart::Chart(const Application::Chart::Chart* chrt) noexcept : Chart { chrt->size, chrt->range, chrt->samples, chrt->f } {

    }

    Chart::Chart(Size<int> sz, pair<float, float> rng, int smpl, function<float(float)> fnc) noexcept :
        size { sz }, range { rng }, samples { smpl }, f { fnc } {

    }

    void Chart::calculatePoints() noexcept {
        auto dx { (range.second - range.first) / (samples - 1) };
        this->points.clear();
        this->points.reserve(samples);
        generate_n(back_inserter(points), samples, [i = 0, &firstX = this->range.first, &f = this->f, dx]() mutable {
            auto x { firstX + i * dx };
            i++;
            return Point<float>{ x, f(x) };
        });

        auto [highestPoint, lowestPoint] { minmax_element(cbegin(points), cend(points),
                                                          [](auto&& a, auto&& b) -> bool { return a.y < b.y; }) };
        auto minY { highestPoint->y };
        auto maxY { lowestPoint->y };

        auto [leftmostPoint, rightmostPoint] { minmax_element(cbegin(points), cend(points),
                                                              [](auto&& a, auto&& b) -> bool { return a.x < b.x; }) };
        auto minX { leftmostPoint->x };
        auto maxX { rightmostPoint->x };

        auto xyRectangle { Rectangle<float>(minX - (maxX - minX) / 18.0f, maxX + (maxX - minX) / 18.0f,
                                            minY - (maxY - minY) / 18.0f, maxY + (maxY - minY) / 18.0f) };

        auto marginRectangle { Rectangle<float>(0, this->size.width, 0, this->size.height)
            .box(Application::chartMarginTop, Application::chartMarginBottom, Application::chartMarginLeft, Application::chartMarginRight) };

        auto xPerPixel { xyRectangle.width() / marginRectangle.width() };
        auto yPerPixel { xyRectangle.height() / marginRectangle.height() };

        auto horizontalLinesCount { 4 };
        auto pixelsPerHorizontalLine { marginRectangle.height() / (horizontalLinesCount + 1) };
        horizontalLines.clear();
        horizontalLabels.clear();
        for (auto i = 1; i <= horizontalLinesCount; i++) {
            auto linePosY { marginRectangle.bottom() - i * pixelsPerHorizontalLine };

            horizontalLines.push_back({ { marginRectangle.left(), linePosY }, { marginRectangle.right(), linePosY } });
            horizontalLabels.push_back({ to_wstring(xyRectangle.bottom() - (linePosY - marginRectangle.top()) * yPerPixel).substr(0,6),
                                       { marginRectangle.right() + 15, linePosY - 5 } });
        }

        auto verticalLinesCount { 6 };
        auto pixelsPerVerticalLine { marginRectangle.width() / (verticalLinesCount + 1) };
        verticalLines.clear();
        verticalLabels.clear();
        for (auto i = 1; i <= verticalLinesCount; i++) {
            auto linePosX { marginRectangle.left() + i * pixelsPerVerticalLine };

            verticalLines.push_back({ { linePosX, marginRectangle.top() }, { linePosX, marginRectangle.bottom() } });
            verticalLabels.push_back({ to_wstring(xyRectangle.left() + (linePosX - marginRectangle.left()) * xPerPixel).substr(0,6),
                                     { linePosX, marginRectangle.bottom() + 5 } });
        }

        transform(cbegin(this->points), cend(this->points), begin(this->points),
                  [marginRectangle, xyRectangle, xPerPixel, yPerPixel](auto&& p) -> Point<float> {
                      return { marginRectangle.left() + ((p.x - xyRectangle.left()) / xPerPixel),
                          marginRectangle.bottom() - (p.y - xyRectangle.top()) / yPerPixel
                      };
                  });
    }

    void Chart::drawLinesAndLabels(const RenderTarget* rt) const noexcept {
        rt->draw(horizontalLines, Application::chartAxisColor, Application::chartLineThickness);

        for (auto&& l : horizontalLabels)
            rt->drawText(l.first, Application::labelSize, l.second);

        rt->draw(verticalLines, Application::chartAxisColor, Application::chartLineThickness);

        for (auto&& l : verticalLabels)
            rt->drawText(l.first, Application::labelSize, l.second);
    }
}