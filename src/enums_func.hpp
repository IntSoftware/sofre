
#ifndef SOFRE_ENUMS_IMPL_HPP
#define SOFRE_ENUMS_IMPL_HPP

#include "core.hpp"
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
            Log::error("Unknown ShaderType enum value!" + std::to_string(static_cast<int>(shaderType)));
            return 0;
    }
}

static std::string shaderTypeName(const ShaderType& shaderType) {
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

inline int toGLCursorMode(const CursorMode& mode) {
    switch (mode) {
        case CursorMode::NORMAL:
            return GLFW_CURSOR_NORMAL;
        case CursorMode::HIDDEN:
            return GLFW_CURSOR_HIDDEN;
        case CursorMode::DISABLED:
            return GLFW_CURSOR_DISABLED;
        case CursorMode::CAPTURED:
            return GLFW_CURSOR_CAPTURED;
        default:
            return GLFW_CURSOR_NORMAL;
    }
}

} // namespace sofre

#endif //SOFRE_ENUMS_IMPL_HPP