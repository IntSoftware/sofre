#include "core.hpp"

#include <sofre/engine.hpp>
#include <sofre/renderer.hpp>
#include <sofre/log.hpp>

#include <vector>

namespace sofre {

struct GraphicEngine::ContextList {
    std::vector<std::unique_ptr<Renderer>> contexts;

    const Renderer* master() {
      if (contexts.empty())
        return nullptr;
      else 
        return contexts.front().get();
    }
};

GraphicEngine& GraphicEngine::instance()
{
    static GraphicEngine engine;
    if (!engine.m_contextList)
        engine.m_contextList = std::make_unique<ContextList>();
    return engine;
}

/**
 * Initialize engine.
 */
bool GraphicEngine::init()
{

    Log::setErrorLogger(Log::defaultErrConsumer);
    Log::setLogger(Log::defaultLogConsumer);

    glfwSetErrorCallback([](int errcode, const char* description) {
        Log::error("GLFW Error [" + std::to_string(errcode) + "] : " + description);
    });

    // Initialise GLFW
    if (glfwInit() != GLFW_TRUE)
    {
        Log::error("Failed to initialize GLFW");
        return false;
    }
    return true;
}

void GraphicEngine::shutdown()
{
    m_contextList->contexts.clear();
    glfwTerminate();
}

Renderer& GraphicEngine::createWindow(const Window& desc)
{
  m_contextList->contexts.emplace_back(
      std::make_unique<Renderer>(desc, m_contextList->master()));
  return *m_contextList->contexts.back();
}

void GraphicEngine::update()
{
    glfwPollEvents();
    for (auto& ctx : m_contextList->contexts)
        ctx->render();
}

bool GraphicEngine::running() const
{
    for (auto& ctx : m_contextList->contexts)
        if (!ctx->shouldClose())
            return true;
    return false;
}

} // namespace sofre
