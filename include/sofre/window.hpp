#ifndef SOFRE_WINDOW_HPP
#define SOFRE_WINDOW_HPP

#include <string>

namespace sofre {

struct Window {
    int width  = 1280;
    int height = 720;
    const char* title = u8"sofre";
    bool vsync = true;

    float aspect() const { return (height > 0) ? (float)width / height : 1.0f; }
};

}

#endif // SOFRE_WINDOW_HPP
