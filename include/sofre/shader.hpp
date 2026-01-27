#ifndef SOFRE_SHADER_HPP
#define SOFRE_SHADER_HPP

#include <sofre/enums.hpp>
#include <sofre/math.hpp>

#include <string>
#include <utility>
#include <filesystem>

namespace sofre {

class Shader {
public:
    static inline Shader fromSource(ShaderType type, std::string source) {
        return Shader{type, source};
    }
    static Shader fromFile(ShaderType type,
                            const std::filesystem::path& path,
                            bool isUTF8withoutBOM = false);

    ~Shader() = default;

    ShaderType type() const { return m_type; }
    const std::string& source() const { return m_source; }
    bool valid() const { return !m_source.empty(); }

    /** in-place replace non-ASCII to space */
    static void utf8_to_ascii_replace(std::string& utf8Str);
    /** write output to parameter 'out' */
    static void utf8_to_ascii(const char* data, size_t size, std::string& out);
    static void utf16_to_ascii(const char* data, size_t size, std::string& out, bool littleEndian);
    static void utf32_to_ascii(const char* data, size_t size, std::string& out, bool littleEndian);

private:
    Shader(ShaderType type, std::string source) : m_type(type), m_source(std::move(source)) {}
    Shader(ShaderType type) : m_type(type) {}

    ShaderType  m_type;
    std::string m_source{};
};

struct Uniform { // TODO : rename to ShaderUniform
    UniformType type;
    union {
        mat4  mat4v;
        float vec3v[3];
        float floatv;
        int   intv;
    };
    bool requiredInShader; //is this uniform must exist in shader code?

    Uniform(UniformType type, bool required) : type(type), requiredInShader(required) {}

    
    Uniform(const mat4& m, bool required = true)
        : type(UniformType::Mat4), mat4v(m), requiredInShader(required) {}

    Uniform(float x, float y, float z, bool required = true)
        : type(UniformType::Vec3), requiredInShader(required) {
        vec3v[0] = x;
        vec3v[1] = y;
        vec3v[2] = z;
    }

    Uniform(float v, bool required = true)
        : type(UniformType::Float), floatv(v), requiredInShader(required) {}

    Uniform(int v, bool required = true)
        : type(UniformType::Int), intv(v), requiredInShader(required) {}
};

} // namespace sofre

#endif
