
#include <iostream>
#include <cmath>
#include <chrono>

#include <sofre/sofre.hpp>

int main() {
    auto& engine = sofre::GraphicEngine::instance();
    if (!engine.init()) {
        sofre::Log::error("Failed to initialize Graphic Engine");
        return -1;
    }

    sofre::Log::log("Example 2, Background color change");
    
    sofre::Window desc;
    desc.title = u8"Example 2, Background color change";
    desc.width = 800;
    desc.height = 600;
    desc.vsync = true;

    auto& renderer = engine.createWindow(desc);

    sofre::Scene scene;
    float vertex[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f 
    };
    auto obj = sofre::Object::create(vertex, sizeof(vertex));
    scene.addObject(obj);

    const char* vertexShader = R"(#version 330 core
        layout (location = 0) in vec3 aPos;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
        }
        )";

    const char* fragmentShader = R"(#version 330 core
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
        )";

    if (!renderer.addShader(sofre::Shader::fromSource(sofre::ShaderType::VERTEX, vertexShader))) {
        sofre::Log::error("Failed to add vertex shader");
        return -1;
    }
    if (!renderer.addShader(sofre::Shader::fromSource(sofre::ShaderType::FRAGMENT, fragmentShader))) {
        sofre::Log::error("Failed to add fragment shader");
        return -1;
    }
    renderer.buildProgram();
    
    auto lastFrame = std::chrono::steady_clock::now();
    auto lastReport = std::chrono::steady_clock::now();
    unsigned int frameCount = 0;
    while (engine.running()) {
        auto now = std::chrono::steady_clock::now();
        lastFrame = now;

        // print FPS every 5 seconds
        std::chrono::duration<float> reportElapsed = now - lastReport;
        if (reportElapsed.count() >= 5.0f) {
            std::cout << "[FPS] " << (int)(frameCount / reportElapsed.count()) << ", "
                      << "[Frame Time] " << (reportElapsed.count() / frameCount * 1000.0f) << " ms"
                      << std::endl;
            lastReport = now;
            frameCount = 0;
        }

        const float timeUnit =
            std::chrono::duration<float>(now.time_since_epoch()).count();
        const float speed = 1.0f;
        const float t = timeUnit * speed;
        renderer.setBackgroundColor(
            (std::sin(t) / 2.0f) + 0.5f,
            (std::sin(t + 2.094f) / 1.0f) + 0.5f, // +120'
            (std::sin(t + 4.188f) / 1.0f) + 0.5f, // +240'
            1.0f
        );

        engine.update(scene);
        frameCount++;
    }

    engine.shutdown();
}

/*
            std::abs(std::sin(timeUnit)),
            std::abs(std::cos(timeUnit)),
            std::abs(std::sin(timeUnit) + std::cos(timeUnit)) / std::sqrt(2.0f),
*/