// src/GraphicEngine.cpp
#include <sofre/engine.hpp>
#include <sofre/renderContext.hpp>
#include <sofre/window.hpp>

#include <GLFW/glfw3.h>
#include <vector>

namespace sofre {

struct GraphicEngine::Impl {
    std::vector<std::unique_ptr<RenderContext>> contexts;
    RenderContext* master = nullptr;
};

GraphicEngine& GraphicEngine::instance()
{
    static GraphicEngine engine;
    if (!engine.m_impl)
        engine.m_impl = std::make_unique<Impl>();
    return engine;
}

bool GraphicEngine::init()
{
    return glfwInit() == GLFW_TRUE;
}

void GraphicEngine::shutdown()
{
    m_impl->contexts.clear();
    glfwTerminate();
}

RenderContext& GraphicEngine::createWindow(const Window& desc)
{
    auto ctx = std::make_unique<RenderContext>();
    ctx->create(desc, m_impl->master);

    if (!m_impl->master)
        m_impl->master = ctx.get();

    m_impl->contexts.emplace_back(std::move(ctx));
    return *m_impl->contexts.back();
}

void GraphicEngine::update()
{
    glfwPollEvents();
    for (auto& ctx : m_impl->contexts)
        ctx->render();
}

bool GraphicEngine::running() const
{
    for (auto& ctx : m_impl->contexts)
        if (!ctx->shouldClose())
            return true;
    return false;
}

} // namespace sofre
