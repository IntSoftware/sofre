#ifndef SOFRE_DEBUG_HPP
#define SOFRE_DEBUG_HPP

#include <string>

namespace sofre {
void dumpGLState();
void checkGLError(const std::string& msg = {});
#define SOFRE_GL_CHECK() checkGLError(std::string("In check ") + __FILE__ + ":" + std::to_string(__LINE__))
}
#endif // SOFRE_DEBUG_HPP
