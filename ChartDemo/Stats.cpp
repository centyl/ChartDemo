#include "Stats.h"

#include <sstream>

namespace Application::Stats {
    using namespace std;
    using namespace std::chrono;

    namespace {
        auto startTime = high_resolution_clock::now();
        long long ms {};
        auto sum { 0 };
        auto samples { 0 };
        auto avg { 0 };
    }

    void begin() noexcept {
        startTime = high_resolution_clock::now();
    }

    void end() noexcept {
        auto endTime = high_resolution_clock::now();
        auto diff { endTime - startTime };
        ms = duration_cast<microseconds>(diff).count();
        sum += ms;
        samples++;
        if (samples == 100) {
            avg = sum / 100;
            sum = 0;
            samples = 0;
        }
    }

    std::wstring getStats() noexcept {
        auto stream = wstringstream {};
        stream << "Last frame render time: ";
        if (ms < 1000)
            stream << "0";
        else
            stream << ms / 1000;
        stream << ".";
        auto tmp { ms % 1000 };
        if (tmp < 100)
            stream << "0";
        if (tmp < 10)
            stream << "0";
        stream << tmp << " ms. 100 frame average: ";
        if (avg < 1000)
            stream << "0";
        else
            stream << avg / 1000;
        stream << ".";
        tmp = avg % 1000;
        if (tmp < 100)
            stream << "0";
        if (tmp < 10)
            stream << "0";
        stream << tmp << " ms";
        return stream.str();
    }

}