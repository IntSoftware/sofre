#ifndef SOFRE_SOFRE_HPP
#define SOFRE_SOFRE_HPP

#include <memory>
#include <functional>

#include <sofre/enums.hpp>

namespace sofre {

class System {
public:
    static int init();
    static void terminate();

};



class GraphicEngine {
    // Engine lifecycle
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;

//    // Window management
//    virtual std::shared_ptr<Window> createWindow(const std::string &title,
//                                                 int width,
//                                                 int height) = 0;

//    virtual void destroyWindow(std::shared_ptr<Window> window) = 0;

    
    virtual void renderFrame() = 0;
    virtual void resize(int width, int height) = 0;

//    virtual void addObject(std::shared_ptr<RenderObject> object) = 0;
//    virtual void removeObject(std::shared_ptr<RenderObject> object) = 0;
    
    void setCursorMode(CursorMode mode);
    void setStickyKey(bool sticky);
};

std::unique_ptr<GraphicEngine> createEngine();

} // namespace sofre

#endif
