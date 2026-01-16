#ifndef SOFRE_ENGINE_HPP
#define SOFRE_ENGINE_HPP

#include <sofre/renderer.hpp>
#include <memory>

namespace sofre {

struct Window;
class Renderer;

class GraphicEngine {
public:
    static GraphicEngine& instance();

    bool init();
    void shutdown();

    Renderer& createWindow(const Window& desc);
    void update(const Scene& scene);

    bool running() const;
    
    //virtual void resize(int width, int height) = 0;
    //void setCursorMode(CursorMode mode);
    //void setStickyKey(bool sticky);
private:
    GraphicEngine() = default;
    ~GraphicEngine() = default;

    GraphicEngine(const GraphicEngine&) = delete;
    GraphicEngine& operator=(const GraphicEngine&) = delete;

private:
    struct ContextList;
    std::unique_ptr<ContextList> m_contextList;
};

} // namespace sofre

#endif // SOFRE_ENGINE_HPP
