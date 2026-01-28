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

    sofre::Scene scene;

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
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

    obj->transform().position = { 0.0f, 0.0f, 0.0f };
    obj->transform().scale    = { 1.0f, 1.0f, 1.0f };

    scene.addObject(obj);

    const char* vertexShader = R"(#version 330 core
        layout (location = 0) in vec3 aPos;

        uniform mat4 model;

        void main()
        {
            gl_Position = model * vec4(aPos, 1.0);
        }
    )";

    const char* fragmentShader = R"(#version 330 core
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
    )";

    if (!renderer.addShader(
            sofre::Shader::fromSource(
                sofre::ShaderType::VERTEX, vertexShader))) {
        sofre::Log::error("Failed to add vertex shader");
        return -1;
    }

    if (!renderer.addShader(
            sofre::Shader::fromSource(
                sofre::ShaderType::FRAGMENT, fragmentShader))) {
        sofre::Log::error("Failed to add fragment shader");
        return -1;
    }

    renderer.buildProgram();

    auto lastReport = std::chrono::steady_clock::now();
    unsigned int frameCount = 0, moveSteps = 0;

    while (engine.running()) {
        auto now = std::chrono::steady_clock::now();

        // FPS 출력
        std::chrono::duration<float> elapsed = now - lastReport;
        if (elapsed.count() >= 5.0f) {
            std::cout << "[FPS] "
                      << (int)(frameCount / elapsed.count())
                      << std::endl;
            lastReport = now;
            frameCount = 0;
        }

        // rotate and move
        auto& transform = obj->transform();
        transform.rotation.z += 0.01f;
        if(moveSteps < 10) {
            transform.position.x += 0.01f;
        } else if(moveSteps < 30) {
            transform.position.y -= 0.01f;
        } else if(moveSteps < 50) {
            transform.position.x -= 0.01f;
        } else if(moveSteps < 70) {
            transform.position.y += 0.01f;
        } else if(moveSteps < 80){
            transform.position.x += 0.01f;
        } else {
            moveSteps = 0;
            transform.position.y = transform.position.x = 0.0f;
        }

        engine.update(scene);
        frameCount++; moveSteps++;
    }

    engine.shutdown();
}
