
#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>

#include <sofre/sofre.hpp>

#include "example_config.hpp"

static const sofre::VertexPC cubeVertices[] = {
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

std::vector<sofre::vec3> generateRandomPositions(int num_cubes, const sofre::Renderer& renderer);
static float randf(float a, float b) { return a + (b - a) * (float(rand()) / float(RAND_MAX)); }

int main(int argc, char* argv[]) {
    int num_cubes = 300;
    if (argc == 2) {
        num_cubes = std::stoi(argv[1]);
    }

    auto& engine = sofre::GraphicEngine::instance();
    if (!engine.init()) {
        sofre::Log::error("Failed to initialize Graphic Engine");
        return -1;
    }

    sofre::Log::log("Example6 - Many Rotating 3D Cubes");

    sofre::Window desc;
    desc.title  = u8"Example6 - Many Cubes";
    desc.width  = 1900;
    desc.height = 1050;

    auto& renderer = engine.createWindow(desc);

    sofre::CameraParams camera({4.0f, 3.0f, -3.0f}, // camera position
                               {0.0f, 0.0f, 0.0f},  // camera is looking...
                               {0.0f, 1.0f, 0.0f},  // camera up
                               45.0f,               // fovY in degree
                               0.1f,                // zNear
                               100.0f               // zFar
    );
    renderer.setCamera(camera);

    sofre::Scene scene;

    // get pre-defined layout
    sofre::VertexLayout layout = sofre::get_vlayout<sofre::VertexPC>();

    auto cubeMesh = sofre::Mesh::create(
        (const void*)cubeVertices,
        sizeof(cubeVertices),
        layout
    );
    auto positions = generateRandomPositions(num_cubes, renderer);
    for (int i = 0; i < num_cubes; ++i) {
        auto cube = sofre::Object::create(cubeMesh);

        cube->transform().position = positions[i];
        cube->transform().scale = {1.0f, 1.0f, 1.0f};
        cube->transform().rotation = {randf(0.0f, 3.14f), randf(0.0f, 3.14f), randf(0.0f, 3.14f)};

        cube->addUniformCallback(
            [cube](const sofre::Program::UniformSetter& u) {
                u.mat4(
                    "model",
                    cube->transform().modelMatrix()
                );
            }
        );
        scene.addObject(cube);
    }

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
        // Rotate cubes
        // -----------------------------
        int cnt = 0;
        for (auto& c : scene.objects()) {
            auto& t = c->transform();
            if (cnt & 1) {
                t.rotation.y += randf(0.007f, 0.013f);
                t.rotation.x += randf(0.012f, 0.018f);
            } else {
                t.rotation.y -= randf(0.007f, 0.013f);
                t.rotation.x -= randf(0.012f, 0.018f);
            }
            cnt++;
        }

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



// -----------------------------
// "Hey, ChatGPT, write a messy function that distributes cubes into random spaces!"
// Following was the result :D
// -----------------------------

// sqrt distribution prevents center-overcrowding
static float randCentered() { return sqrtf(sqrtf(randf(0.0f, 1.0f))); }

// -----------------------------
// random position generating inside camera frustum
// -----------------------------
std::vector<sofre::vec3> generateRandomPositions(int num_cubes, const sofre::Renderer& renderer) {
    std::vector<sofre::vec3> positions;
    positions.reserve(num_cubes);

    const auto& cam = renderer.camera().cam3d;
    const auto& win = renderer.windowDescription();

    // camera param
    sofre::vec3 eye = cam.eye;
    sofre::vec3 center = cam.center;
    sofre::vec3 up = cam.up;

    float fovY = cam.fovY;
    float zNear = cam.zNear;
    float zFar = cam.zFar;

    float aspect = float(win.width) / float(win.height);

    float tanHalfFov = tanf(fovY * 0.5f * 3.14159265f / 180.0f);

    // camera basis
    sofre::vec3 forward = glm::normalize(center - eye);
    sofre::vec3 right = glm::normalize(cross(forward, up));
    sofre::vec3 camUp = glm::cross(right, forward);

    for (int i = 0; i < num_cubes; ++i) {
        sofre::vec3 pos;

        int tries = 0;
        do {
            // -----------------------------
            // (1) depth : remove too close
            // -----------------------------
            float minDepth = zNear + 5.0f; // not too near
            float d = randf(minDepth, zFar - 1.0f);

            // -----------------------------
            // (2) frustum size in the depth
            // -----------------------------
            float h = 2.0f * d * tanHalfFov;
            float w = h * aspect;

            // -----------------------------
            // (3) wide distribution
            // -----------------------------
            float rx = randCentered();
            float ry = randCentered();

            float x = randf(-1.0f, 1.0f) * (w * 0.5f) * rx;
            float y = randf(-1.0f, 1.0f) * (h * 0.5f) * ry;

            // -----------------------------
            // (4) world position
            // -----------------------------
            pos = eye + forward * d + right * x + camUp * y;

            tries++;
            if (tries > 10)
                break;

        } while (false); // no restriction in strong collision

        positions.push_back(pos);
    }

    return positions;
}