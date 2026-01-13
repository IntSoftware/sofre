
#include <iostream>

#include <sofre/sofre.hpp>

int main() {
    auto& engine = sofre::GraphicEngine::instance();
    if (!engine.init()) {
        sofre::Log::error("Failed to initialize Graphic Engine");
        return -1;
    }

    sofre::Log::log("Hello, Window!");
    
    sofre::Window desc;
    desc.title = u8"Hello Window";

    engine.createWindow(desc);

    while (engine.running())
    {
        engine.update();
    }

    engine.shutdown();
}
