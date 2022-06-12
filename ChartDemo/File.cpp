#include "File.h"

#include <fstream>

namespace Application::Util {

    using namespace std;

    string readFile(const string& filename) {
        auto fileStream { ifstream { filename } };
        auto it { istreambuf_iterator<char> { fileStream } };
        return string { it, istreambuf_iterator<char>{} };
    }

}