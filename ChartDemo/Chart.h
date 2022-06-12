#pragma once

#include <functional>
#include <string>

#include "Drawable.h"

namespace Application::Chart {

    class Chart : public Application::Rendering::Drawable {
        public:

        void setSize(Application::Util::Size<int> sz) noexcept;

        void setFunction(std::function<float(float)> fnc) noexcept;

        void setRange(std::pair<float, float> rng) noexcept;

        void setSamples(int smpl) noexcept;

        virtual ~Chart() noexcept = default;
        Chart(const Chart& chrt) = delete;
        Chart& operator=(const Chart& chrt) = delete;
        Chart(Chart&& chrt) = delete;
        Chart& operator=(Chart&& chrt) = delete;

        protected:

        void calculatePoints() noexcept;

        void drawLinesAndLabels(const Application::Rendering::RenderTarget* rt) const noexcept;

        Chart(const Application::Chart::Chart* chrt) noexcept;

        Chart(Application::Util::Size<int> sz, std::pair<float, float> rng, int smpl, std::function<float(float)> fnc) noexcept;

        std::vector<Application::Util::Point<float>> points {};

        std::vector<Application::Util::Line<float>> horizontalLines {};
        std::vector<Application::Util::Line<float>> verticalLines {};

        std::vector<std::pair<std::wstring, Application::Util::Point<float>>> horizontalLabels {};
        std::vector<std::pair<std::wstring, Application::Util::Point<float>>> verticalLabels {};

        Application::Util::Size<int> size { 0,0 };
        std::function<float(float)> f { [](float x) -> float { return x; } };
        std::pair<float, float> range { 0,0 };
        int samples { 0 };
    };

}

