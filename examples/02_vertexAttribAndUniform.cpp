
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

    sofre::Log::log("Example 2, Vertex attribute and uniform");
    
    sofre::Window desc;
    desc.title = u8"Example 2, Vertex attribute and uniform";
    desc.width = 800;
    desc.height = 600;
    desc.vsync = true;

    auto& renderer = engine.createWindow(desc);
    renderer.setCamera(sofre::CameraParams::None());

    sofre::Scene scene;
    // position (x,y,z) + color (r,g,b)
    float vertex[] = {
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // red
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, // green
         0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f  // blue
    };
    sofre::VertexLayout layout;
    layout.stride = sizeof(float) * 6; // 6 floats per vertex object
    layout.attributes = {
        {
            0, // location = 0
            3, // vec3
            sofre::VertexAttribType::Float,
            false,
            0  //offset = 0
        },
        {
            1, // location = 1
            3, // vec3
            sofre::VertexAttribType::Float,
            false,
            sizeof(float) * 3 // offset : 3 floats
        }
    };

    auto mesh = sofre::Mesh::create(
        (const void*)vertex,
        sizeof(vertex),
        layout
    );

    auto obj = sofre::Object::create(mesh);

    float colorMagnitude = 0.01f;
    obj->addUniformCallback([&colorMagnitude](const sofre::Program::UniformSetter& us) {
        us.float1("uMagnitude", colorMagnitude); // set uniform every frame
    });
    scene.addObject(obj);

    const char* vertexShader = R"(#version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;

        out vec3 vColor;

        void main()
        {
            vColor = aColor;
            gl_Position = vec4(aPos, 1.0);
        }
        )";

    const char* fragmentShader = R"(#version 330 core
        in vec3 vColor;
        out vec4 FragColor;

        uniform float uMagnitude;

        void main()
        {
            FragColor = vec4(vColor * uMagnitude, 1.0);
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
    unsigned int frameCount = 0;
    const float interval = 5.0f;
    while (engine.running()) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> reportElapsed = now - lastFrame;

        // Change color magnitude
        colorMagnitude = 0.5f + 0.5 * std::sin(reportElapsed.count() / interval * 3.141592f);

        // print FPS every 5 seconds
        if (reportElapsed.count() >= interval) {
            std::cout << "[FPS] " << (int)(frameCount / reportElapsed.count()) << ", "
                      << "[Frame Time] " << (reportElapsed.count() / frameCount * 1000.0f) << " ms"
                      << std::endl;
            lastFrame = now;
            frameCount = 0;
        }

        engine.update(scene);
        frameCount++;
    }

    engine.shutdown();
}