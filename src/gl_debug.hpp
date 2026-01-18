#ifndef SOFRE_GL_DEBUG_HPP
#define SOFRE_GL_DEBUG_HPP

#include "core.hpp" // For OpenGL headers

#include <string>

namespace sofre::gl {

std::string glErrorToString(GLenum err);
void checkError(const char* file, int line);
#define SOFRE_GL_CHECK() ::sofre::gl::checkError(__FILE__, __LINE__)

#ifdef SOFRE_DEBUG
void initDebug();
#else
inline void initDebug() {}
#endif // SOFRE_DEBUG

} // namespace sofre::gl

#endif // SOFRE_GL_DEBUG_HPP