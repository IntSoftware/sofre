#include <sofre/program.hpp>
#include <sofre/shader.hpp>
#include <sofre/log.hpp>
#include <sofre/math.hpp>

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

bool Program::addShader(ShaderType type, std::string_view source) {
    if (source.empty()) {
        Log::error("Invalid shader source(source is empty)!");
        Log::error("Maybe loading shader source file is failed?");
        return false;
    }
    GLenum glType = toGLShaderType(type);
    GLuint id = glCreateShader(glType);

    const char* src = source.data();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    GLint ok = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    GLint logLength = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        char* log = new char[logLength];

        glGetShaderInfoLog(id, logLength, nullptr, log);
        Log::error("[Shader compile info log] [Type: " +
                   shaderTypeName(type) + "]");
        Log::error(log);
        delete[] log;
    }

    if (!ok) {
        Log::error("Failed to compile shader!");
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
    GLint logLength = 0;
    glGetProgramiv(gl->m_program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        char* log = new char[logLength];

        glGetShaderInfoLog(gl->m_program, logLength, nullptr, log);

        Log::error("[Program link info log]");
        Log::error(log);
        delete[] log;
    }

    for (const auto& shaderID : gl->m_shaders)
        glDeleteShader(shaderID);

    const auto& uniform = uniformSetter();
    m_hasProjMatrix = uniform.exists(shader::builtin_projMatrix);
    m_hasViewMatrix = uniform.exists(shader::builtin_viewMatrix);

    return linked == GL_TRUE;
}

void Program::use() const { glUseProgram(gl->m_program); }


/*    Uniform Setter    */

bool Program::UniformSetter::exists(const char* name) const {
    return glGetUniformLocation(m_program.gl->m_program, name) != -1;
}

bool Program::UniformSetter::mat4(const char* name, const sofre::mat4& v, bool required) const {
    GLint loc = glGetUniformLocation(m_program.gl->m_program, name);
    if (loc == -1) {
        if (required)
            Log::error(std::string("Uniform not found: ") + name);
        return false;
    }
    glUniformMatrix4fv(loc, 1, GL_FALSE, v.m);
    return true;
}

bool Program::UniformSetter::vec3(const char* name, float x, float y, float z, bool required) const {
    GLint loc = glGetUniformLocation(m_program.gl->m_program, name);
    if (loc == -1) {
        if (required)
            Log::error(std::string("Uniform not found: ") + name);
        return false;
    }
    glUniform3f(loc, x, y, z);
    return true;
}

bool Program::UniformSetter::float1(const char* name, float v, bool required) const {
    GLint loc = glGetUniformLocation(m_program.gl->m_program, name);
    if (loc == -1) {
        if (required)
            Log::error(std::string("Uniform not found: ") + name);
        return false;
    }
    glUniform1f(loc, v);
    return true;
}

bool Program::UniformSetter::int1(const char* name, int v, bool required) const {
    GLint loc = glGetUniformLocation(m_program.gl->m_program, name);
    if (loc == -1) {
        if (required)
            Log::error(std::string("Uniform not found: ") + name);
        return false;
    }
    glUniform1i(loc, v);
    return true;
}

} // namespace sofre
