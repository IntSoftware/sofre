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

    void setMat4(const char* name, const mat4& mat) const;
    void setVec3(const char* name, float x, float y, float z) const;
    void setVec3(const char* name, const vec3& vec) const{
        setVec3(name, vec.x, vec.y, vec.z);
    }
    void setFloat(const char* name, float v) const;
    void setInt(const char* name, int v) const;

private:
    struct Program_GL;
    Program_GL* gl;
};

} // namespace sofre

#endif
