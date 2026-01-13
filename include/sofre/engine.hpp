#ifndef SOFRE_ENGINE_HPP
#define SOFRE_ENGINE_HPP

#include <memory>

namespace sofre {

struct Window;
class RenderContext;

class GraphicEngine {
public:
    static GraphicEngine& instance();

    bool init();
    void shutdown();

    RenderContext& createWindow(const Window& desc);
    void update();

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
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace sofre

#endif // SOFRE_ENGINE_HPP
