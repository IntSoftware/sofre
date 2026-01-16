
#include <iostream>

#include <sofre/sofre.hpp>

int main() {
    auto& engine = sofre::GraphicEngine::instance();
    if (!engine.init()) {
        sofre::Log::error("Failed to initialize Graphic Engine");
        return -1;
    }

    sofre::Log::log("Hello, Window!");
    
    sofre::Window desc;
    desc.title = u8"Hello Window";
    desc.width = 800;
    desc.height = 600;

    auto& renderer = engine.createWindow(desc);

    sofre::Scene scene;
    float vertex[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f 
    };
    auto objId = scene.createObject(vertex, sizeof(vertex));
    auto& obj = scene.get(objId);

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
            FragColor = vec4(1.0, 0.4, 0.2, 1.0);
        }
        )";

    sofre::Shader vertShader(sofre::ShaderType::VERTEX, vertexShader);
    sofre::Shader fragShader(sofre::ShaderType::FRAGMENT, fragmentShader);

    if (!renderer.addShader(vertShader)) {
        sofre::Log::error("Failed to add vertex shader");
        return -1;
    }
    if (!renderer.addShader(fragShader)) {
        sofre::Log::error("Failed to add fragment shader");
        return -1;
    }
    renderer.buildProgram();
    
    while (engine.running())
    {
        engine.update(scene);
    }

    engine.shutdown();
}
