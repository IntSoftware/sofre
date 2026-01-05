#include <sgi/sgi.hpp>
#include <sofre/sofre.hpp>

namespace sofre {

    class OpenGLGraphicEngineGlobal
        : public spear::sgi::GraphicEngineGlobal
    {
    public:
        bool initialize() override {
            return 1;
        }

        void shutdown() override {
            //glfwTerminate();
        }
    };

    OpenGLGraphicEngineGlobal g_instance;

}

spear::sgi::GraphicEngineGlobal& spear::sgi::graphic_global() {
	return sofre::g_instance;
}
