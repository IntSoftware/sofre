#ifndef SOFRE_PROGRAM_HPP
#define SOFRE_PROGRAM_HPP

#include <sofre/math.hpp>

namespace sofre {

class Shader;

class Program {
public:
    Program();
    ~Program();

    bool addShader(const Shader& shader);
    bool build();
    bool hasViewMatrix() const { return m_hasViewMatrix; }
    bool hasProjMatrix() const { return m_hasProjMatrix; }
    void use() const;

    /** Uniform functions */
    class UniformSetter {
    public:
        bool mat4(const char* name, const mat4& v, bool required = true) const;
        bool vec3(const char* name, float x, float y, float z, bool required = true) const;
        bool vec3(const char* name, const sofre::vec3& vec, bool required = true) const {
            return vec3(name, vec.x, vec.y, vec.z, required);
        }
        bool float1(const char* name, float v, bool required = true) const;
        bool int1(const char* name, int v, bool required = true) const;

        bool exists(const char* name) const;

    private:
        friend class Program;
        explicit UniformSetter(Program& program) : m_program(program) {}
        Program& m_program;
    };

    UniformSetter uniformSetter() { return UniformSetter(*this); }

private:
    struct Program_GL;
    Program_GL* gl;

    bool m_hasViewMatrix, m_hasProjMatrix;
};


} // namespace sofre

#endif
