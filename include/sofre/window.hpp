#ifndef SOFRE_WINDOW_HPP
#define SOFRE_WINDOW_HPP

#include <string>

namespace sofre {

struct Window {
    int width  = 1280;
    int height = 720;
    std::string title = "sofre";
    bool vsync = true;
};

}

#endif // SOFRE_WINDOW_HPP
