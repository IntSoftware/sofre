#ifndef SOFRE_PROGRAM_HPP
#define SOFRE_PROGRAM_HPP

#include <sofre/math.hpp>
#include <sofre/shader.hpp>
#include <sofre/enums.hpp>

namespace sofre {

class Program {
public:
    Program();
    ~Program();

    bool addShader(const Shader& shader);
    bool build();
    void use() const;

    /** Uniform functions */
    bool uniformExists(const char* name) const;
    bool setUniform(const char* name, const Uniform& value) const;

private:
    struct Program_GL;
    Program_GL* gl;
};

} // namespace sofre

#endif
