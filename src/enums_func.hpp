
#ifndef SOFRE_ENUMS_IMPL_HPP
#define SOFRE_ENUMS_IMPL_HPP

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <sofre/enums.hpp>
#include <sofre/log.hpp>

namespace sofre {
    
inline GLenum toGLShaderType(const ShaderType& shaderType) {
    switch (shaderType) {
        case ShaderType::VERTEX:
            return GL_VERTEX_SHADER;
        case ShaderType::FRAGMENT:
            return GL_FRAGMENT_SHADER;
        case ShaderType::GEOMETRY:
            return GL_GEOMETRY_SHADER;
        default:
            Log::err() << "Unknown ShaderType enum value!" << static_cast<int>(shaderType);
            return 0;
    }
}

inline std::string shaderTypeName(const ShaderType& shaderType) {
    switch (shaderType) {
        case ShaderType::VERTEX:
            return "VERTEX";
        case ShaderType::FRAGMENT:
            return "FRAGMENT";
        case ShaderType::GEOMETRY:
            return "GEOMETRY";
        default:
            return "UNKNOWN_SHADER_TYPE";
    }
}

} // namespace sofre

#endif //SOFRE_ENUMS_IMPL_HPP