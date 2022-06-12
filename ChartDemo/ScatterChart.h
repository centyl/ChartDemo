#pragma once

#include "Chart.h"

namespace Application::Chart {

    class ScatterChart : public Application::Chart::Chart {
        public:

        void draw(const Application::Rendering::RenderTarget* rt) const noexcept override;

        ScatterChart(const Application::Chart::Chart* chrt) noexcept;

        ScatterChart(Application::Util::Size<int> sz, std::pair<float, float> rng, int smpl, std::function<float(float)> fnc) noexcept;
    };

}