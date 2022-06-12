#pragma once

#include <GL/glew.h>
#include <string_view>
#include <fstream>

namespace Application::OpenGL {

    enum class ShaderType {
        ComputeShader = GL_COMPUTE_SHADER,
        VertexShader = GL_VERTEX_SHADER,
        TessControlShader = GL_TESS_CONTROL_SHADER,
        TessEvaluationShader = GL_TESS_EVALUATION_SHADER,
        GeometryShader = GL_GEOMETRY_SHADER,
        FragmentShader = GL_FRAGMENT_SHADER
    };

    class Shader {
        public:
        GLuint getShader() const noexcept;

        Shader(ShaderType t, std::string_view source);

        ~Shader() noexcept;
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&& sh) noexcept;
        Shader& operator=(Shader&& sh) noexcept;

        private:
        ShaderType type;

        GLuint shader;
    };
}

