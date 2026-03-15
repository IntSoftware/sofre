#include <iostream>
#include <iomanip>
#include <chrono>

#include <sofre/sofre.hpp>

#include "example_config.hpp"

int main() {

    auto& engine = sofre::GraphicEngine::instance();
    if (!engine.init()) {
        sofre::Log::error("Failed to initialize Graphic Engine");
        return -1;
    }

    sofre::Log::log("Example4 - Rotating 3D Cube with texture and obj file");

    sofre::Window desc;
    desc.title  = u8"OBJ & Texture Example";
    desc.width  = 800;
    desc.height = 600;

    auto& renderer = engine.createWindow(desc);
    sofre::Scene scene;

    // -----------------------------
    // OBJ mesh
    // -----------------------------

    auto cubeMesh = sofre::Mesh::loadOBJFile(ASSET_DIR/"cube.obj");
    auto cube = sofre::Object::create(cubeMesh);

    // -----------------------------
    // Texture
    // -----------------------------

    auto diceTex = sofre::Texture2D::make_texture(ASSET_DIR/"dice.bmp");

    cube->addTexture("uTexture", diceTex);

    cube->addUniformCallback(
        [cube](const sofre::Program::UniformSetter& u) {
            u.mat4(
                "model",
                cube->transform().modelMatrix()
            );
        }
    );

    cube->transform().scale = {2,2,2};
    scene.addObject(cube);

    // -----------------------------
    // Shader
    // -----------------------------

    const char* vertexShader = R"(
        #version 330 core

        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec2 aTex;

        uniform mat4 model;
        uniform mat4 sofre_ViewMatrix;
        uniform mat4 sofre_ProjMatrix;

        out vec2 vTex;

        void main() {
            vTex = aTex;

            gl_Position = sofre_ProjMatrix * sofre_ViewMatrix * model * vec4(aPos,1.0);
        }
    )";

    const char* fragmentShader = R"(
        #version 330 core
        in vec2 vTex;
        uniform sampler2D uTexture;
        out vec4 FragColor;

        void main() {
            FragColor = texture(uTexture, vTex);
        }
    )";

    if (!renderer.addShader(sofre::ShaderType::VERTEX, vertexShader))
        return -1;

    if (!renderer.addShader(sofre::ShaderType::FRAGMENT, fragmentShader))
        return -1;

    renderer.buildProgram();

    sofre::CameraParams camera(
        {4.0f, 3.0f, -3.0f},
        {0.0f, 0.0f,  0.0f},
        {0.0f, 1.0f,  0.0f},
        45.0f,
        0.1f,
        100.0f
    );

    renderer.setCamera(camera);

    // -----------------------------
    // Loop (same timing structure as Example4)
    // -----------------------------

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
        transform.rotation.y += 0.01f;
        transform.rotation.x += 0.015f;

        engine.update(scene);
        frameCount++;
#if SOFRE_MEASURE_RENDERTIME
        renderTime += renderer.renderTime();
#else
        renderTime = -1.0f;
#endif
    }

    engine.shutdown();
}