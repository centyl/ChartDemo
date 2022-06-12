#include <Windows.h>
#include <stdexcept>

#include "Application.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow) {
    try {
        Application::initialize(hInstance);
    }
    catch (const std::runtime_error& e) {
        return -1;
    }

    return Application::messageLoop();
}
