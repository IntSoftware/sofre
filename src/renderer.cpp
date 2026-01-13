
#include "core.hpp"

#include <sofre/renderer.hpp>
#include <sofre/window.hpp>
#include <sofre/log.hpp>

namespace sofre {


Renderer::Renderer(const Window& desc, const Renderer* master)
{
    m_creat_success = false;

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(
        desc.width,
        desc.height,
        desc.title,
        nullptr,
        master ? master->m_window : nullptr
    );

    if (!m_window)
        return;

    glfwMakeContextCurrent(m_window);

    if (!master) {
        // Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
        const int gladVersion = gladLoadGL(glfwGetProcAddress);
        if (gladVersion == 0)
        {
            Log::error("Failed to initialize OpenGL context!");
            return;
        }

        // Successfully loaded OpenGL
        Log::log(
            "Loaded OpenGL " +
            std::to_string(GLAD_VERSION_MAJOR(gladVersion)) +
            "." +
            std::to_string(GLAD_VERSION_MINOR(gladVersion))
        );
    }
    glfwSwapInterval(desc.vsync ? 1 : 0);

    m_creat_success = true;
}

Renderer::~Renderer()
{
    if (m_window)
        glfwDestroyWindow(m_window);
}

void Renderer::render()
{
    if (!m_window)
        return;

    glfwMakeContextCurrent(m_window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(m_window);
}

bool Renderer::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

} // namespace sofre
