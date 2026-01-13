#ifndef SOFRE_SOFRE_HPP
#define SOFRE_SOFRE_HPP

#include <memory>
#include <functional>

#include <sofre/engine.hpp>
#include <sofre/renderContext.hpp>
#include <sofre/window.hpp>
#include <sofre/enums.hpp>

namespace sofre {

class System {
public:
    static int init();
    static void terminate();

};

} // namespace sofre

#endif
