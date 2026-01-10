#ifdef SOFRE_ENABLE_SGI
/* //TODO : implementation
#include <spear/graphic/interface.hpp>
#include <sofre/sofre.hpp>

namespace sofre {

class OpenGLGraphicEngineGlobal
    : public spear::graphics::GraphicEngineGlobal
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


namespace {

class SgiGraphicEngineWrapper final
    : public spear::graphics::GraphicEngine
{
public:
    explicit SgiGraphicEngineWrapper()
        : impl_(std::make_unique<sofre::GraphicEngine>()) {}

    bool initialize() override {
        return impl_->initialize();
    }

    void shutdown() override {
        impl_->shutdown();
    }

    std::shared_ptr<spear::graphics::Window>
    createWindow(const std::string& title,
                 int width,
                 int height) override
    {
        return impl_->createWindow(title, width, height);
    }

    void destroyWindow(std::shared_ptr<spear::graphics::Window> window) override {
        impl_->destroyWindow(window);
    }

    void renderFrame() override {
        impl_->renderFrame();
    }

    void resize(int width, int height) override {
        impl_->resize(width, height);
    }

    void addObject(std::shared_ptr<spear::graphics::RenderObject> object) override {
        impl_->addObject(object);
    }

    void removeObject(std::shared_ptr<spear::graphics::RenderObject> object) override {
        impl_->removeObject(object);
    }

private:
    std::unique_ptr<sofre::GraphicEngine> impl_;
};

} // anonymous namespace

}

namespace spear::graphics {


GraphicEngineGlobal& global() {
    static sofre::OpenGLGraphicEngineGlobal instance;
    return instance;
}


std::unique_ptr<GraphicEngine> createGraphicEngine() {
    return std::make_unique<sofre::SgiGraphicEngineWrapper>();
}

} // namespace spear::sgi
*/
#endif // SOFRE_ENABLE_SGI

