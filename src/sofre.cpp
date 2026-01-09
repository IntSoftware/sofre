#include "core.hpp"
#include <sofre/sofre.hpp>

#include <iostream>
#include <memory>
#include <string>

namespace sofre {

/**
 * Initialize sofre.
 * return value :
 *  - -2 : glfw init failed
 *  - -3 : glad init failed
 */
int System::init() {
    
    setErrorLogger(Log::defaultLogConsumer);
    setLogger(Log::defaultLogConsumer);

    // Initialise GLFW
    if (!glfwInit())
    {
        System::log_err("Failed to initialize GLFW\n");
        return -2;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
    int gladVersion = gladLoadGL(glfwGetProcAddress);
    if (gladVersion == 0)
    {
        System::log_err("Failed to initialize OpenGL context\n");
        return -3;
    }

    // Successfully loaded OpenGL
    System::log_out(
        "Loaded OpenGL " +
        std::to_string(GLAD_VERSION_MAJOR(gladVersion)) +
        "." +
        std::to_string(GLAD_VERSION_MINOR(gladVersion))
    );

}

void System::terminate() {
    glfwTerminate();
}





class Window;
class RenderObject;

class GraphicEngineImpl {
public:
    bool initialize();
    void shutdown();

    std::shared_ptr<Window> createWindow(const std::string& title,
                                         int width,
                                         int height);
    void destroyWindow(std::shared_ptr<Window> window);

    void renderFrame();
    void resize(int width, int height);

    void addObject(std::shared_ptr<RenderObject> object);
    void removeObject(std::shared_ptr<RenderObject> object);
};


} // namespace sofre
