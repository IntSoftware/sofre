
#include <iostream>

#include <spear/graphic/interface.hpp>

int main() {
    std::cout << "Hello, Window! - with spear-graphic-interface" << std::endl;
    
    spear::graphics::global().initialize();

    auto engine = spear::graphics::createGraphicEngine();
    engine->initialize();

    auto window = engine->createWindow("Hello", 800, 600);

    while (true) {
        engine->renderFrame();
    }

    engine->shutdown();
    spear::graphics::global().shutdown();
}
