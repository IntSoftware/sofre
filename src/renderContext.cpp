// src/RenderContext.cpp
#include <sofre/renderContext.hpp>
#include <sofre/window.hpp>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace sofre {

struct RenderContext::Impl {
    GLFWwindow* window = nullptr;
};

RenderContext::RenderContext()
    : m_impl(new Impl)
{
}

RenderContext::~RenderContext()
{
    if (m_impl->window)
        glfwDestroyWindow(m_impl->window);
    delete m_impl;
}

bool RenderContext::create(const Window& desc, RenderContext* shared)
{
    GLFWwindow* share = shared ? shared->m_impl->window : nullptr;

    m_impl->window = glfwCreateWindow(
        desc.width,
        desc.height,
        desc.title.c_str(),
        nullptr,
        share
    );

    if (!m_impl->window)
        return false;

    glfwMakeContextCurrent(m_impl->window);

    if (!shared)
    {
        if (!gladLoadGL(glfwGetProcAddress))
            return false;
    }

    glfwSwapInterval(desc.vsync ? 1 : 0);
    return true;
}

void RenderContext::render()
{
    if (!m_impl->window)
        return;

    glfwMakeContextCurrent(m_impl->window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(m_impl->window);
}

bool RenderContext::shouldClose() const
{
    return glfwWindowShouldClose(m_impl->window);
}

} // namespace sofre
