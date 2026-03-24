#include "core.hpp"
#include "gl_debug.hpp"

#include <sofre/debug.hpp>
#include <sofre/log.hpp>

using namespace sofre::gl;

namespace sofre {
void dumpGLState() {
    // framebuffer state
    GLenum fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    Log::error("[OpenGL state dump]");
    Log::error("  Framebuffer status: " + glErrorToString(fbStatus));

    // bound texture and VBO info
    GLint bound = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &bound);
    if (bound != 0) {
        GLint linkStatus = 0;
        glGetProgramiv(bound, GL_LINK_STATUS, &linkStatus);
        GLint infoLogLength = 0;
        glGetProgramiv(bound, GL_INFO_LOG_LENGTH, &infoLogLength);
        Log::error("  Current shader program ID: " + std::to_string(bound) +
                   ", LinkStatus: " + (linkStatus == GL_TRUE ? "OK" : "FAIL"));
        std::string infoLog(infoLogLength, '\0');
        if (infoLogLength > 0) {
            glGetProgramInfoLog(bound, infoLogLength, nullptr, infoLog.data());
            Log::error("  Program InfoLog: " + infoLog);
        }
    } else {
        Log::error("  No shader program bound");
    }
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound);
    if (bound != 0) {
        GLint width = 0, height = 0, internalFormat = 0;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);
        Log::error("  Current 2D texture bound (ID: " + std::to_string(bound) + ") Size: " + std::to_string(width) + "x" +
                   std::to_string(height) + ", Format: " + std::to_string(internalFormat));
    } else {
        Log::error("  No 2D texture bound");
    }
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &bound);
    Log::error("  Current VAO bound: " + std::to_string(bound));
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &bound);
    Log::error("  Current VBO bound: " + std::to_string(bound));
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &bound);
    Log::error("  Current EBO bound: " + std::to_string(bound));
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    Log::error("  Viewport: x=" + std::to_string(vp[0]) + ", y=" + std::to_string(vp[1]) +
               ", w=" + std::to_string(vp[2]) + ", h=" + std::to_string(vp[3]));
}

void checkGLError(const std::string& msg) {
    GLenum err;
    if ((err = glGetError()) != GL_NO_ERROR) {
        if(msg.size()) Log::error(msg);
        Log::error("OpenGL Error : " + glErrorToString(err));
        dumpGLState();
    }
}
}