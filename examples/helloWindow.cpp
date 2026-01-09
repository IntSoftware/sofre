
#include <iostream>

#include <sgi/sgi.hpp>

int main() {
    std::cout << "Hello, Window!" << std::endl;
    
    spear::grapics::global().initialize();

    auto engine = spear::sgi::createGraphicEngine();
    engine->initialize();

    auto window = engine->createWindow("Hello", 800, 600);

    while (...) {
        engine->renderFrame();
    }

    engine->shutdown();
    spear::sgi::graphic_global().shutdown();
}
