#ifndef SOFRE_PROGRAM_HPP
#define SOFRE_PROGRAM_HPP

#include <vector>

namespace sofre {

class Shader;

class Program {
public:
    Program();
    ~Program();

    bool addShader(const Shader& shader);
    bool build();
    void use() const;

private:
    struct Program_GL;
    Program_GL* gl;
};

} // namespace sofre

#endif
