#include "enums_func.hpp"
#include "os_detect.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <sofre/renderer.hpp>
#include <sofre/window.hpp>
#include <sofre/object.hpp>
#include <sofre/log.hpp>
#include <sofre/shader.hpp>

#include <sofre/texture2d.hpp>

#include <glutil/debug.hpp>
#include <glutil/debug_info.hpp>

#include <list>
#include <memory>
#include <vector>
#include <chrono>

#if SOFRE_MEASURE_RENDERTIME
#include <chrono> // check rendering time
#endif
namespace sofre {

struct Renderer::Renderer_Impl {
    ~Renderer_Impl() {
        destroy();
    }

    void destroy() {
        if (m_window)
            glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    GLint maxTextureUnits = -1;
    GLFWwindow* m_window = nullptr;
    std::list<std::shared_ptr<Object>> objectList;
    std::vector<std::unique_ptr<Scene>> scenes;
    std::vector<SceneHandle> freeSceneHandles;
};

Renderer::Renderer(const Window& desc, int glversion) : m_view(), m_proj(), m_windowDesc(desc){
    m_creat_success = false;
    impl = new Renderer_Impl();

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glversion / 10);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glversion % 10);
    if(IsMacOS) glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #if SOFRE_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    #endif

    impl->m_window = glfwCreateWindow(
        m_windowDesc.width,
        m_windowDesc.height,
        m_windowDesc.title,
        nullptr,
        nullptr
    );

    if (!impl->m_window)
        return;

    glfwSetWindowUserPointer(impl->m_window, this);
    glfwMakeContextCurrent(impl->m_window);

    const int gladVersion = gladLoadGL(glfwGetProcAddress);
    if (gladVersion == 0) {
        Log::err() << "Failed to initialize OpenGL context!";
        return;
    }

    // Initialize glutil's debug system
    glutil::debug::init();

    // Log runtime OpenGL info via glutil (routed through sofre's logging)
    glutil::debug::printRuntimeInfo(false, Log::outStream());

    setBackgroundColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(impl->m_window, &fbWidth, &fbHeight);
    glfwSetFramebufferSizeCallback(impl->m_window, [](GLFWwindow* window, int width, int height) {
        ((Renderer*)glfwGetWindowUserPointer(window))->resize(width, height);
    });
    glfwSwapInterval(desc.vsync ? 1 : 0);

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &impl->maxTextureUnits);

    m_creat_success = true;
}

Renderer::~Renderer() {
    destroy();
    delete impl;
}

void Renderer::destroy() {
    if (!impl->m_window)
        return;

    glfwMakeContextCurrent(impl->m_window);
    impl->scenes.clear();
    impl->freeSceneHandles.clear();
    m_program.destroy();

    glfwDestroyWindow(impl->m_window);
    impl->m_window = nullptr;
}

void Renderer::setCamera(const CameraParams& params) {
    m_camera = params;
}

void Renderer::resize(int width, int height) {
    m_windowDesc.width  = width;
    m_windowDesc.height = height;
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(impl->m_window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
}

void Renderer::setBackgroundColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Renderer::addObject(const std::shared_ptr<Object>& obj) {
    impl->objectList.push_back(obj);
}

void Renderer::removeObject(const std::shared_ptr<Object>& obj) {
    impl->objectList.remove(obj);
}

Scene& Renderer::createScene() {
    auto scene = std::make_unique<Scene>();

    SceneHandle handle;
    if (!impl->freeSceneHandles.empty()) {
        handle = impl->freeSceneHandles.back();
        impl->freeSceneHandles.pop_back();
        impl->scenes[handle] = std::move(scene);
    } else {
        handle = static_cast<SceneHandle>(impl->scenes.size());
        impl->scenes.push_back(std::move(scene));
    }

    impl->scenes[handle]->initialize(handle);
    return *impl->scenes[handle];
}

Scene* Renderer::scene(SceneHandle handle) {
    if (handle >= impl->scenes.size())
        return nullptr;
    return impl->scenes[handle].get();
}

const Scene* Renderer::scene(SceneHandle handle) const {
    if (handle >= impl->scenes.size())
        return nullptr;
    return impl->scenes[handle].get();
}

bool Renderer::removeScene(SceneHandle handle) {
    if (handle >= impl->scenes.size() || !impl->scenes[handle])
        return false;

    impl->scenes[handle]->destroy();
    impl->scenes[handle].reset();
    impl->freeSceneHandles.push_back(handle);
    return true;
}

bool Renderer::activateScene(SceneHandle handle) {
    auto* scene = this->scene(handle);
    if (!scene)
        return false;
    scene->activate();
    return true;
}

bool Renderer::deactivateScene(SceneHandle handle) {
    auto* scene = this->scene(handle);
    if (!scene)
        return false;
    scene->deactivate();
    return true;
}

bool Renderer::isSceneActive(SceneHandle handle) const {
    const auto* scene = this->scene(handle);
    return scene ? scene->active() : false;
}

void Renderer::clearActiveScenes() {
    for (auto& scene : impl->scenes)
        if (scene)
            scene->deactivate();
}

void Renderer::renderSceneObjects(const Scene& scene, const Program::UniformSetter& uniforms) {
    for (const auto& obj : scene.objects()) {
        int texUnit = 0;

        for (const auto& t : obj->textureBindings()) {
            if (texUnit > impl->maxTextureUnits) {
                Log::err() << "Max texture unit reached : " << impl->maxTextureUnits;
                Log::err() << "Unable to bind texture \"" << t.uniform << "\" which should've been " << texUnit;
                break;
            }
            t.texture->bind(texUnit);
            uniforms.int1(t.uniform.c_str(), texUnit);
            texUnit++;
        }

        obj->applyUniforms(uniforms);
        obj->mesh().draw();
    }
}

void Renderer::render() {
    if (!m_creat_success || !impl->m_window)
        return;

    if (glfwGetCurrentContext() != impl->m_window)
        glfwMakeContextCurrent(impl->m_window);

#if SOFRE_MEASURE_RENDERTIME
    auto start_time = std::chrono::high_resolution_clock::now();
#endif
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program.use();
    auto uniforms = m_program.uniformSetter();

    if (m_camera.type != CameraMode::None) {
        if (m_program.hasViewMatrix()) {
            m_camera.computeView(m_view);
            uniforms.mat4(shader::builtin_viewMatrix, m_view, false);
        }

        if (m_program.hasProjMatrix()) {
            m_camera.computeProj(m_proj, m_windowDesc.aspect());
            uniforms.mat4(shader::builtin_projMatrix, m_proj, false);
        }
    }

    for (const auto& scene : impl->scenes) {
        if (!scene || !scene->active())
            continue;
        renderSceneObjects(*scene, uniforms);
    }

#if SOFRE_MEASURE_RENDERTIME
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    m_renderTime = duration.count() / 1000.0f;
#endif

    glfwSwapBuffers(impl->m_window);
}

bool Renderer::shouldClose() const {
    return glfwWindowShouldClose(impl->m_window);
}

} // namespace sofre
