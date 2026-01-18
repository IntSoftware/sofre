#include "core.hpp" // For OpenGL headers

#include "gl_debug.hpp"

#include <sofre/log.hpp>

#include <string>
#include <sstream>

namespace sofre::gl {

std::string glErrorToString(GLenum err) {
    std::stringstream ss;
    switch (err) {
        case GL_NO_ERROR: ss << "GL_NO_ERROR"; break;
        case GL_INVALID_ENUM: ss << "GL_INVALID_ENUM"; break;
        case GL_INVALID_VALUE: ss << "GL_INVALID_VALUE"; break;
        case GL_INVALID_OPERATION: ss << "GL_INVALID_OPERATION"; break;
        case GL_OUT_OF_MEMORY: ss << "GL_OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: ss << "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
        default: ss << "UNKNOWN_ERROR";
    }
    ss << '(' << err << ')';
    return ss.str();
}

void checkError(const char* file, int line) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        Log::error(glErrorToString(err) + " detacted at " + file + ":" +
                   std::to_string(line));
    }
}

#if defined(GL_VERSION_4_3) || defined(GL_KHR_debug)
static void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                        const GLchar* message, const void* userParam) {
    // https://learnopengl.com/In-Practice/Debugging
    // https://gist.github.com/Challanger524/cdf90cf11809749363fb638646225773
    std::stringstream ss;
    ss << "opengl debug message callback invoked!\n";
    ss << "---------------------gldebugCallback-start----------------\n";
    ss << "Message: " << message << '\n';
    ss << "ID: " << id << '\n';
    ss << "Source: ";
    switch (source) {
        case GL_DEBUG_SOURCE_API: ss << "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: ss << "WINDOW_SYSTEM"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: ss << "SHADER_COMPILER"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: ss << "THIRD_PARTY"; break;
        case GL_DEBUG_SOURCE_APPLICATION: ss << "APPLICATION"; break;
        case GL_DEBUG_SOURCE_OTHER: ss << "OTHER"; break;
        default: ss << "unknown source (" << source << ')'; break;
    }
    ss << '\n';
    ss << "Type: ";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR: ss << "ERROR"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: ss << "DEPRECATED_BEHAVIOR"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: ss << "UNDEFINED_BEHAVIOR"; break;
        case GL_DEBUG_TYPE_PORTABILITY: ss << "PORTABILITY"; break;
        case GL_DEBUG_TYPE_PERFORMANCE: ss << "PERFORMANCE"; break;
        case GL_DEBUG_TYPE_MARKER: ss << "MARKER"; break;
        case GL_DEBUG_TYPE_OTHER: ss << "OTHER"; break;
        default: ss << "unknown type (" << type << ')'; break;
    }
    ss << '\n';
    ss << "Severity: ";
    switch (severity) {
        case GL_DEBUG_SEVERITY_LOW: ss << "LOW"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: ss << "MEDIUM"; break;
        case GL_DEBUG_SEVERITY_HIGH: ss << "HIGH"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: ss << "NOTIFICATION"; break;
        default: ss << "unknown severity (" << severity << ')'; break;
    }
    ss << '\n';
    ss << "---------------------gldebugCallback-end------------------";
    Log::error(ss.str());
}
#endif // GL_VERSION_4_3

#ifdef SOFRE_DEBUG
static void postGLfuncCallback(void *ret, const char *name, GLADapiproc apiproc, int len_args, ...) {
    GLenum err;
    while ((err = glad_glGetError()) != GL_NO_ERROR) {
        Log::error(std::string("[GL ERROR] ") + glErrorToString(err) + " in function " + name);
    }
}

void initDebug() {
    #if defined(GL_VERSION_4_3) || defined(GL_KHR_debug)
    if (GLAD_GL_KHR_debug || GLAD_GL_VERSION_4_3) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debugCallback, nullptr);
    } else {
        Log::error("OpenGL debug output not supported");
    }
    #else
    Log::log("OpenGL debug output not available at compile time(GL_VERSION_4_3 or GL_KHR_debug not defined)");
    Log::log("Using glad post callback for error checking...");
    gladSetGLPostCallback(postGLfuncCallback);
    #endif // GL_VERSION_4_3
}
#endif // SOFRE_DEBUG

} // namespace sofre::gl
