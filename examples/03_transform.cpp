#include <iostream>
#include <chrono>

#include <sofre/sofre.hpp>

int main() {
    auto& engine = sofre::GraphicEngine::instance();
    if (!engine.init()) {
        sofre::Log::error("Failed to initialize Graphic Engine");
        return -1;
    }

    sofre::Log::log("Example3 - Mesh / Object / Transform");

    sofre::Window desc;
    desc.title = u8"Example3 - Object & Transform";
    desc.width = 800;
    desc.height = 600;

    auto& renderer = engine.createWindow(desc);
    renderer.setCamera(
        sofre::CameraParams(
            -desc.aspect(), desc.aspect(),   // left, right
            -1.0f, 1.0f,   // bottom, top
            -1.0f, 1.0f    // zNear, zFar
        )
    );

    sofre::Scene scene;

    float vertices[] = {
        -0.4330126941f, -0.25f, 0.0f, // sqrt(3)/4
         0.4330126941f, -0.25f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    auto mesh = sofre::Mesh::create(vertices, sizeof(vertices));
    auto obj = sofre::Object::create(mesh);
    obj->addUniformCallback(
        [obj](const sofre::Program::UniformSetter& u) {
            u.mat4(
                "model",
                obj->transform().modelMatrix()
            );
        }
    );

    auto& transform = obj->transform();
    transform.position = { 0.0f, 0.0f, 0.0f };
    transform.scale    = { 1.0f, 1.0f, 1.0f };

    scene.addObject(obj);

    const char* vertexShader = R"(#version 330 core
        layout (location = 0) in vec3 aPos;

        uniform mat4 model;
        uniform mat4 sofre_ViewMatrix;
        uniform mat4 sofre_ProjMatrix;

        void main()
        {
            gl_Position = sofre_ProjMatrix * sofre_ViewMatrix * model * vec4(aPos, 1.0);
        }
    )";

    const char* fragmentShader = R"(#version 330 core
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0);
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

    auto lastFrame = std::chrono::steady_clock::now();
    unsigned int frameCount = 0;
    while (engine.running()) {
        auto now = std::chrono::steady_clock::now();

        // print FPS every 5 seconds
        std::chrono::duration<float> reportElapsed = now - lastFrame;
        if (reportElapsed.count() >= 5.0f) {
            std::cout << "[FPS] " << (int)(frameCount / reportElapsed.count()) << ", "
                      << "[Frame Time] " << (reportElapsed.count() / frameCount * 1000.0f) << " ms"
                      << std::endl;
            lastFrame = now;
            frameCount = 0;
        }

        // rotate and move
        static float t = 0.0f;
        t += 0.01f;
        transform.position.x = 0.3f * std::cos(t);
        transform.position.y = 0.3f * std::sin(t);

        engine.update(scene);
        frameCount++;
    }

    engine.shutdown();
}
