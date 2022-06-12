#pragma once

#include <GL/glew.h>

#include "Shader.h"

namespace Application::OpenGL {
    class Program {
        public:

        void attach(Application::OpenGL::Shader* shader) const noexcept;

        void link() const;

        void validate() const;

        void use() const noexcept;

        GLint getUniformLocation(std::string_view name) const noexcept;

        Program();

        ~Program() noexcept;
        Program(const Program&) = delete;
        Program& operator=(const Program&) = delete;
        Program(Program&& p) noexcept;
        Program& operator=(Program&& p) noexcept;

        private:
        GLuint program { };

    };
}

