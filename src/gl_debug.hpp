#ifndef SOFRE_GL_DEBUG_HPP
#define SOFRE_GL_DEBUG_HPP

#include "core.hpp" // For OpenGL headers

#include <string>
#include <set>

namespace sofre::gl {

void dumpGLState();
std::string glErrorToString(GLenum err);
void checkGLErrorInFileLine(const char* file, int line);
#define SOFRE_GL_CHECK() ::sofre::gl::checkGLErrorInFileLine(__FILE__, __LINE__)

#if SOFRE_DEBUG
extern std::set<std::string> functionCheckWhiteList, functionCheckBlackList;
inline std::set<std::string>& glFunctionCheckWhiteList() { return functionCheckWhiteList; }
inline std::set<std::string>& glFunctionCheckBlackList() { return functionCheckBlackList; }

void initDebug();
#else
inline void initDebug() {}
#endif // SOFRE_DEBUG


// max GL_VERSION_* macro
inline const char* maxDefinedGLVersionSymbol() {
#if defined(GL_VERSION_4_6)
    return "GL_VERSION_4_6";
#elif defined(GL_VERSION_4_5)
    return "GL_VERSION_4_5";
#elif defined(GL_VERSION_4_4)
    return "GL_VERSION_4_4";
#elif defined(GL_VERSION_4_3)
    return "GL_VERSION_4_3";
#elif defined(GL_VERSION_4_2)
    return "GL_VERSION_4_2";
#elif defined(GL_VERSION_4_1)
    return "GL_VERSION_4_1";
#elif defined(GL_VERSION_4_0)
    return "GL_VERSION_4_0";
#elif defined(GL_VERSION_3_3)
    return "GL_VERSION_3_3";
#elif defined(GL_VERSION_3_2)
    return "GL_VERSION_3_2";
#elif defined(GL_VERSION_3_1)
    return "GL_VERSION_3_1";
#elif defined(GL_VERSION_3_0)
    return "GL_VERSION_3_0";
#elif defined(GL_VERSION_2_1)
    return "GL_VERSION_2_1";
#elif defined(GL_VERSION_2_0)
    return "GL_VERSION_2_0";
#else
    return "GL_VERSION_UNKNOWN";
#endif
}

} // namespace sofre::gl

#endif // SOFRE_GL_DEBUG_HPP