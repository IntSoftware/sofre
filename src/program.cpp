#include <sofre/program.hpp>
#include <sofre/shader.hpp>

#include "enums_func.hpp"
#include "core.hpp"

#include <vector>

namespace sofre {

struct Program::Program_GL {
    ~Program_GL() {
        glDeleteProgram(m_program);
    }
    void createProgram() {
        glDeleteProgram(m_program);
        m_program = glCreateProgram();
    }
    GLuint m_program = 0;
    std::vector<GLuint> m_shaders;
};

Program::Program() { gl = new Program_GL(); }

Program::~Program() { delete gl; }

bool Program::addShader(const Shader& shader) {
    GLenum glType = toGLShaderType(shader.type());
    GLuint id = glCreateShader(glType);

    const char* src = shader.source().c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    GLint ok = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (!ok) { // TODO : get error log
        glDeleteShader(id);
        return false;
    }

    gl->m_shaders.push_back(id);
    return true;
}

bool Program::build() {
    gl->createProgram();

    for (const auto& shaderID : gl->m_shaders) {
        glAttachShader(gl->m_program, shaderID);
    }

    glLinkProgram(gl->m_program);

    GLint linked = 0;
    glGetProgramiv(gl->m_program, GL_LINK_STATUS, &linked);

    for (const auto& shaderID : gl->m_shaders)
        glDeleteShader(shaderID);

    return linked == GL_TRUE;
}

void Program::use() const { glUseProgram(gl->m_program); }

} // namespace sofre
