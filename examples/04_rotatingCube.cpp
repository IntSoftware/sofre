
#include <iostream>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include <sofre/sofre.hpp>

#include "example_config.hpp"

sofre::mat4 toSofreMat4(const glm::mat4& glmMat) {
    sofre::mat4 mat;
    const float* src = glm::value_ptr(glmMat);
    for (int i = 0; i < 16; ++i) {
        mat.m[i] = src[i];
    }
    return mat;
}

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

        uniform mat4 sofre_ModelMatrix;
        uniform mat4 sofre_ViewMatrix;
        uniform mat4 sofre_ProjMatrix;

        out vec3 vColor;

        void main() {
            vColor = aColor;
            gl_Position = sofre_ProjMatrix * sofre_ViewMatrix
                        * sofre_ModelMatrix * vec4(aPos, 1.0);
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

    // -----------------------------
    // Camera matrices (fixed)
    // -----------------------------
    glm::mat4 view =  glm::lookAt(
        glm::vec3(4, 3, -3),    // Camera position in World Space
        glm::vec3(0, 0, 0),    // and looks at the origin
        glm::vec3(0, 1, 0)     // Head is up (set to 0,-1,0 to look upside-down)
    );
    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f),
        desc.width / (float)desc.height,
        0.1f, 100.0f
    );

    renderer.setViewMatrix(toSofreMat4(view));
    renderer.setProjectionMatrix(toSofreMat4(proj));

    // -----------------------------
    // Main loop
    // -----------------------------
    auto lastReport = std::chrono::steady_clock::now();
    unsigned int frameCount = 0;

    while (engine.running()) {
        auto now = std::chrono::steady_clock::now();

        std::chrono::duration<float> elapsed = now - lastReport;
        if (elapsed.count() >= 5.0f) {
            std::cout << "[FPS] "
                      << (int)(frameCount / elapsed.count())
                      << std::endl;
            lastReport = now;
            frameCount = 0;
        }

        // -----------------------------
        // Rotate cube
        // -----------------------------
        auto& transform = cube->transform();
        transform.rotation.y += 0.01f;   // Y-axis rotation
        transform.rotation.x += 0.015f;   // X-axis rotation
        cube->setUniform(
            "sofre_ModelMatrix",
            sofre::Uniform(cube->transform().modelMatrix())
        );

        engine.update(scene);
        frameCount++;
    }

    engine.shutdown();
}
