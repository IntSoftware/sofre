#ifndef SOFRE_PROGRAM_HPP
#define SOFRE_PROGRAM_HPP

#include <sofre/math.hpp>
#include <sofre/enums.hpp>
#include <sofre/shader.hpp>

#include <filesystem>

namespace sofre {

class Program {
public:
    Program();
    ~Program();

    bool addShader(const Shader& shader);
    template<typename... Args>
    bool addShader(Args&&... args) {return addShader(Shader{std::forward<Args>(args)...});}
    bool addShaderFromFile(ShaderType type, const std::filesystem::path& path, bool isUTF8withoutBOM = false) {
        return addShader({type, Shader::readFile(path, isUTF8withoutBOM)});
    }
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
