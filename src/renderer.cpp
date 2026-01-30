#include "core.hpp"
#include "enums_func.hpp"
#include "gl_debug.hpp"

#include <sofre/renderer.hpp>
#include <sofre/window.hpp>
#include <sofre/object.hpp>
#include <sofre/log.hpp>
#include <sofre/shader.hpp>

#include <list>
#include <memory>

namespace sofre {

struct Renderer::Renderer_GL {
    Renderer_GL() {}
    ~Renderer_GL() {
        if (m_window)
            glfwDestroyWindow(m_window);
    }

    GLFWwindow* m_window = nullptr;
    std::list<std::shared_ptr<Object>> objectList;
};

Renderer::Renderer(const Window& desc, const Renderer* master) : m_view(), m_proj(), m_windowDesc(desc){
    m_creat_success = false;
    m_windowDesc = desc;
    gl = new Renderer_GL();

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #ifdef SOFRE_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    #endif

    gl->m_window = glfwCreateWindow(
        m_windowDesc.width,
        m_windowDesc.height,
        m_windowDesc.title,
        nullptr,
        master ? master->gl->m_window : nullptr
    );

    if (!gl->m_window)
        return;

    glfwSetWindowUserPointer(gl->m_window, this);
    glfwMakeContextCurrent(gl->m_window);

    if (!master) {
        // Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
        const int gladVersion = gladLoadGL(glfwGetProcAddress);
        if (gladVersion == 0){
            Log::error("Failed to initialize OpenGL context!");
            return;
        }

        // Successfully loaded OpenGL
        Log::log("GLAD loaded OpenGL : " +
                 std::to_string(GLAD_VERSION_MAJOR(gladVersion)) + "." +
                 std::to_string(GLAD_VERSION_MINOR(gladVersion)));
        Log::log("OpenGL connection : " + std::string((const char*)glGetString(GL_VERSION)));
        Log::log("GLSL language version : " + std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));
        Log::log("Vendor : " + std::string((const char*)glGetString(GL_VENDOR))
                + ", Renderer : " + std::string((const char*)glGetString(GL_RENDERER)));
    }
    gl::initDebug();
    

    // dark blue background
    setBackgroundColor(0.0f, 0.0f, 0.4f, 0.0f); //TODO : set background color configurable
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(gl->m_window, &fbWidth, &fbHeight);
    glfwSetFramebufferSizeCallback(gl->m_window, [](GLFWwindow* window, int width, int height) {
        ((Renderer*)glfwGetWindowUserPointer(window))->resize(width, height);
    });
    glfwSwapInterval(desc.vsync ? 1 : 0);

    m_creat_success = true;
}


Renderer::~Renderer() { delete gl; }

void Renderer::setCamera(const CameraParams& params) {
    m_camera = params;
}

void Renderer::resize(int width, int height) {
    m_windowDesc.width  = width;
    m_windowDesc.height = height;
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(gl->m_window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
}

void Renderer::setBackgroundColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Renderer::addObject(const std::shared_ptr<Object>& obj) {
    gl->objectList.push_back(obj);
}

void Renderer::removeObject(const std::shared_ptr<Object>& obj) {
    gl->objectList.remove(obj);
}

void Renderer::render(const Scene& scene) {
    if (!gl->m_window)
        return;
        
    glfwMakeContextCurrent(gl->m_window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_program.use();
    auto uniforms = m_program.uniformSetter();
    if (m_camera.type != CameraMode::None) {
        if (m_program.hasViewMatrix()) {
            m_camera.computeView(m_view);
            uniforms.mat4(Shader::builtin_viewMatrix, m_view, false);
        }

        if (m_program.hasProjMatrix()) {
            m_camera.computeProj(m_proj, m_windowDesc.aspect());
            uniforms.mat4(Shader::builtin_projMatrix, m_proj, false);
        }
    }

    for (const auto& obj : scene.objects()) {
        obj->applyUniforms(uniforms);
        obj->mesh().draw();
    }

    glfwSwapBuffers(gl->m_window);
}

bool Renderer::shouldClose() const {
    return glfwWindowShouldClose(gl->m_window);
}



} // namespace sofre
