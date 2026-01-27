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

bool Program::addShader(const Shader& shader) {
    if(!shader.valid()) {
        Log::error("Invalid shader source(source is empty)!");
        Log::error("Maybe loading shader source file is failed?");
        return false;
    }
    GLenum glType = toGLShaderType(shader.type());
    GLuint id = glCreateShader(glType);

    const char* src = shader.source().c_str();
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
                   shaderTypeName(shader.type()) + "]");
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

    return linked == GL_TRUE;
}

void Program::use() const { glUseProgram(gl->m_program); }

bool Program::uniformExists(const char* name) const {
    if (!gl->m_program)
        return false;

    return glGetUniformLocation(gl->m_program, name) != -1;
}

bool Program::setUniform(const char* name, const Uniform& value) const {
    if (!gl->m_program)
        return false;

    GLint loc = glGetUniformLocation(gl->m_program, name);
    if (loc == -1 && value.requiredInShader) {
        Log::error(std::string("Uniform not found: ") + name);
        return false;
    }

    switch (value.type) {
    case UniformType::Mat4:
        glUniformMatrix4fv(loc, 1, GL_FALSE, value.mat4v.m);
        break;

    case UniformType::Vec3:
        glUniform3f(loc,
            value.vec3v[0],
            value.vec3v[1],
            value.vec3v[2]);
        break;

    case UniformType::Float:
        glUniform1f(loc, value.floatv);
        break;

    case UniformType::Int:
        glUniform1i(loc, value.intv);
        break;
    default:
        Log::error(std::string("Unknown uniform type for uniform : ") + name);
        return false;
    }
    return true;
}

} // namespace sofre
