
#include "core.hpp"
#include "enums_impl.hpp"

#include <sofre/renderer.hpp>
#include <sofre/window.hpp>
#include <sofre/object.hpp>
#include <sofre/log.hpp>

namespace sofre {

struct Renderer::Renderer_GL {
    Renderer_GL() {}
    ~Renderer_GL() {
        glDeleteProgram(m_program);

        if (m_window)
            glfwDestroyWindow(m_window);
    }

    GLFWwindow* m_window = nullptr;
    GLuint m_program = 0;
};

Renderer::Renderer(const Window& desc, const Renderer* master)
{
    m_creat_success = false;
    gl = new Renderer_GL();

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    gl->m_window = glfwCreateWindow(
        desc.width,
        desc.height,
        desc.title,
        nullptr,
        master ? master->gl->m_window : nullptr
    );

    if (!gl->m_window)
        return;

    glfwMakeContextCurrent(gl->m_window);

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
    delete gl;
}

void Renderer::addObject(const std::shared_ptr<Object>& obj) {
    objectList.push_back(obj);
}

void Renderer::removeObject(const std::shared_ptr<Object>& obj) {
    objectList.remove(obj);
}

void Renderer::render()
{
    if (!gl->m_window)
        return;

    glfwMakeContextCurrent(gl->m_window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(gl->m_program);

    for (const auto& obj : objectList) {
        obj->bind();
        glDrawArrays(GL_TRIANGLES, 0, obj->vertexCount());
    }

    glfwSwapBuffers(gl->m_window);
}

bool Renderer::shouldClose() const
{
    return glfwWindowShouldClose(gl->m_window);
}



} // namespace sofre
