#include "ScatterChart.h"
#include "Constants.h"

namespace Application::Chart {

    using namespace std;
    using namespace Application::Util;
    using namespace Application::Rendering;

    void ScatterChart::draw(const RenderTarget* rt) const noexcept {
        drawLinesAndLabels(rt);

        rt->draw(this->points, 2, Application::chartFunctionColor);
    }

    ScatterChart::ScatterChart(const Application::Chart::Chart* chrt) noexcept : Chart { chrt } {
        this->calculatePoints();
    }

    ScatterChart::ScatterChart(Size<int> sz, pair<float, float> rng, int smpl, function<float(float)> fnc) noexcept : Chart { sz, rng, smpl, fnc } {
        this->calculatePoints();
    }

}