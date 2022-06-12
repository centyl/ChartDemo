#include "LineChart.h"
#include "Constants.h"

namespace Application::Chart {

    using namespace std;
    using namespace Application::Rendering;
    using namespace Application::Util;

    void LineChart::draw(const RenderTarget* rt) const noexcept {
        drawLinesAndLabels(rt);

        rt->drawLineStrip(points, Application::chartFunctionColor, Application::chartFunctionThickness);
    }

    LineChart::LineChart(const Chart* chrt) noexcept : Chart { chrt } {
        this->calculatePoints();
    }

    LineChart::LineChart(Size<int> sz, pair<float, float> rng, int smpl, function<float(float)> fnc) noexcept : Chart { sz, rng, smpl, fnc } {
        this->calculatePoints();
    }

}