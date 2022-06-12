#include "Shader.h"

#include <stdexcept>

namespace Application::OpenGL {

    using namespace std;

    GLuint Shader::getShader() const noexcept {
        return shader;
    }

    Shader::Shader(ShaderType t, string_view source) : type { t }, shader { glCreateShader((GLenum) t) } {
        if (!this->shader)
            throw runtime_error { "Creating shader failed" };

        auto q { source.data() };
        auto r { (GLint) source.length() };

        glShaderSource(shader, 1, &q, &r);
        glCompileShader(shader);

        auto result = GLint {};
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
            throw runtime_error { "Compiling shader failed" };
    }

    Shader::~Shader() noexcept {
        glDeleteShader(shader);
    }

    Shader::Shader(Shader&& sh) noexcept : shader { exchange(sh.shader,0) } {
    }

    Shader& Shader::operator=(Shader&& sh) noexcept {
        glDeleteShader(shader);
        this->shader = exchange(sh.shader, 0);
        return *this;
    }

}