#pragma once

#include <Windows.h>

namespace Application {

    void initialize(HINSTANCE instance);

    int messageLoop() noexcept;

}