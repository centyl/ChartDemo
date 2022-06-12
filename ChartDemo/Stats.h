#pragma once

#include<chrono>
#include <string>

namespace Application::Stats {

    void begin() noexcept;

    void end() noexcept;

    std::wstring getStats() noexcept;

};

