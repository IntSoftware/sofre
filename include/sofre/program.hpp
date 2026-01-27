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
    void use() const;

    /** Uniform functions */
    bool uniformExists(const char* name) const;
    bool setMat4(const char* name, const mat4& mat, bool required = true) const;
    bool setVec3(const char* name, float x, float y, float z, bool required = true) const;
    bool setVec3(const char* name, const vec3& vec, bool required = true) const{
        return setVec3(name, vec.x, vec.y, vec.z, required);
    }
    bool setFloat(const char* name, float v, bool required = true) const;
    bool setInt(const char* name, int v, bool required = true) const;

private:
    struct Program_GL;
    Program_GL* gl;
};

} // namespace sofre

#endif
