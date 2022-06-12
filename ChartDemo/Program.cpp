#include "Program.h"

#include <stdexcept>

namespace Application::OpenGL {

    using namespace std;

    Program::Program() : program { glCreateProgram() } {
        if (!this->program)
            throw runtime_error { "Creating shader program failed" };
    }

    Program::~Program() noexcept {
        glDeleteProgram(program);
    }

    Program::Program(Program&& p) noexcept : program { exchange(p.program, 0) } {

    }

    Program& Program::operator=(Program&& p) noexcept {
        glDeleteProgram(this->program);
        this->program = exchange(p.program, 0);
        return *this;
    }

    void Program::attach(Shader* shader) const noexcept {
        glAttachShader(this->program, shader->getShader());
    }

    void Program::link() const {
        glLinkProgram(this->program);
        auto result = GLint {};
        glGetProgramiv(program, GL_LINK_STATUS, &result);
        if (result == GL_FALSE)
            throw runtime_error { "Linking program failed" };
    }

    void Program::validate() const {
        glValidateProgram(this->program);
        auto result = GLint {};
        glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
        if (result == GL_FALSE)
            throw runtime_error { "Validating program failed" };
    }

    void Program::use() const noexcept {
        glUseProgram(this->program);
    }

    GLint Program::getUniformLocation(string_view name) const noexcept {
        return glGetUniformLocation(this->program, name.data());
    }

}