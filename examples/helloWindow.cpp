
#include <iostream>

#include <sofre/sofre.hpp>

int main() {
    std::cout << "Hello, Window!" << std::endl;
    
    auto& engine = sofre::GraphicEngine::instance();
    if (!engine.init())
        return -1;

    sofre::Window desc;
    desc.title = "Hello sofre Window";

    engine.createWindow(desc);

    while (engine.running())
    {
        engine.update();
    }

    engine.shutdown();
}
