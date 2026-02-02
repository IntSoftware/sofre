#include <iostream>
#include <iomanip>
#include <chrono>

#include <sofre/sofre.hpp>

#include "example_config.hpp"

struct VertexPC {
    float px, py, pz;   // position
    float r, g, b;      // color
};

static const VertexPC cubeVertices[] = {
    // front
    {-0.5f,-0.5f, 0.5f, 0,1,0}, { 0.5f,-0.5f, 0.5f, 0,1,0}, { 0.5f, 0.5f, 0.5f, 1,1,1},
    {-0.5f,-0.5f, 0.5f, 0,1,0}, { 0.5f, 0.5f, 0.5f, 1,1,1}, {-0.5f, 0.5f, 0.5f, 1,1,0},

    // back
    {-0.5f,-0.5f,-0.5f, 0,0,0}, {-0.5f, 0.5f,-0.5f, 1,0,0}, { 0.5f, 0.5f,-0.5f, 1,0,1},
    {-0.5f,-0.5f,-0.5f, 0,0,0}, { 0.5f, 0.5f,-0.5f, 1,0,1}, { 0.5f,-0.5f,-0.5f, 0,0,1},

    // left
    {-0.5f,-0.5f,-0.5f, 0,0,0}, {-0.5f,-0.5f, 0.5f, 0,1,0}, {-0.5f, 0.5f, 0.5f, 1,1,0},
    {-0.5f,-0.5f,-0.5f, 0,0,0}, {-0.5f, 0.5f, 0.5f, 1,1,0}, {-0.5f, 0.5f,-0.5f, 1,0,0},

    // right
    { 0.5f,-0.5f,-0.5f, 0,0,1}, { 0.5f, 0.5f,-0.5f, 1,0,1}, { 0.5f, 0.5f, 0.5f, 1,1,1},
    { 0.5f,-0.5f,-0.5f, 0,0,1}, { 0.5f, 0.5f, 0.5f, 1,1,1}, { 0.5f,-0.5f, 0.5f, 0,1,0},

    // top
    {-0.5f, 0.5f,-0.5f, 1,0,0}, {-0.5f, 0.5f, 0.5f, 1,1,0}, { 0.5f, 0.5f, 0.5f, 1,1,1},
    {-0.5f, 0.5f,-0.5f, 1,0,0}, { 0.5f, 0.5f, 0.5f, 1,1,1}, { 0.5f, 0.5f,-0.5f, 1,0,1},

    // bottom
    {-0.5f,-0.5f,-0.5f, 0,0,0}, { 0.5f,-0.5f,-0.5f, 0,0,1}, { 0.5f,-0.5f, 0.5f, 0,1,0},
    {-0.5f,-0.5f,-0.5f, 0,0,0}, { 0.5f,-0.5f, 0.5f, 0,1,0}, {-0.5f,-0.5f, 0.5f, 0,1,0},
};


int main() {
    auto& engine = sofre::GraphicEngine::instance();
    if (!engine.init()) {
        sofre::Log::error("Failed to initialize Graphic Engine");
        return -1;
    }

    sofre::Log::log("Example4 - Rotating 3D Cube");

    sofre::Window desc;
    desc.title  = u8"Example4 - Rotating Cube";
    desc.width  = 800;
    desc.height = 600;

    auto& renderer = engine.createWindow(desc);
    sofre::Scene scene;

    sofre::VertexLayout layout;
    layout.stride = sizeof(VertexPC);
    layout.attributes = {
        {
            0,                          // location = 0
            3,                          // vec3
            sofre::VertexAttribType::Float,
            false,
            offsetof(VertexPC, px)
        },
        {
            1,                          // location = 1
            3,                          // vec3
            sofre::VertexAttribType::Float,
            false,
            offsetof(VertexPC, r)
        }
    };

    auto cubeMesh = sofre::Mesh::create(
        (const void*)cubeVertices,
        sizeof(cubeVertices),
        layout
    );

    auto cube = sofre::Object::create(cubeMesh);
    cube->addUniformCallback(
        [cube](const sofre::Program::UniformSetter& u) {
            u.mat4(
                "model",
                cube->transform().modelMatrix()
            );
        }
    );

    // initial transform
    cube->transform().position = { 0.0f, 0.0f, 0.0f };
    cube->transform().scale    = { 2.0f, 2.0f, 2.0f };

    scene.addObject(cube);

    // -----------------------------
    // Shader
    // -----------------------------
    const char* vertexShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;

        uniform mat4 model;
        uniform mat4 sofre_ViewMatrix;
        uniform mat4 sofre_ProjMatrix;

        out vec3 vColor;

        void main() {
            vColor = aColor;
            gl_Position = sofre_ProjMatrix * sofre_ViewMatrix
                        * model * vec4(aPos, 1.0);
        }
    )";

    const char* fragmentShader = R"(
        #version 330 core
        in vec3 vColor;
        out vec4 FragColor;

        void main() {
            FragColor = vec4(vColor, 1.0);
        }
    )";

    if (!renderer.addShader(sofre::ShaderType::VERTEX, vertexShader)) {
        sofre::Log::error("Failed to add vertex shader");
        return -1;
    }

    if (!renderer.addShader(sofre::ShaderType::FRAGMENT, fragmentShader)) {
        sofre::Log::error("Failed to add fragment shader");
        return -1;
    }

    renderer.buildProgram();

    sofre::CameraParams camera(
        {4.0f, 3.0f, -3.0f},   // camera position
        {0.0f, 0.0f,  0.0f},   // camera is looking...
        {0.0f, 1.0f,  0.0f},   // camera up
        45.0f,                 // fovY in degree
        0.1f,                  // zNear
        100.0f                 // zFar
    );
    renderer.setCamera(camera);

    auto lastFrame = std::chrono::steady_clock::now();
    unsigned int frameCount = 0;
    double renderTime = 0.0;
    while (engine.running()) {
        auto now = std::chrono::steady_clock::now();

        std::chrono::duration<float> elapsed = now - lastFrame;
        if (elapsed.count() >= 5.0f) {
            auto frameTime_avg = (elapsed.count() / frameCount * 1000.0f);
            auto renderTime_avg = (renderTime / frameCount);
            std::cout << "[FPS] " << (int)(frameCount / elapsed.count())
                      << ", [Render Time / Frame Time] "
                      << std::fixed << std::setprecision(4)
                      << renderTime_avg << " / " << frameTime_avg << " ms ("
                      << std::setprecision(2)
                      << (100.0 * renderTime_avg / frameTime_avg) << "%)"
                      << std::endl;
            lastFrame = now;
            frameCount = 0;
            renderTime = 0;
        }

        // -----------------------------
        // Rotate cube
        // -----------------------------
        auto& transform = cube->transform();
        transform.rotation.y += 0.01f;   // Y-axis rotation
        transform.rotation.x += 0.015f;   // X-axis rotation

        engine.update(scene);
        frameCount++;
        renderTime += renderer.renderTime();
    }

    engine.shutdown();
}
