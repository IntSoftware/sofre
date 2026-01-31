#ifndef SOFRE_RENDERCONTEXT_HPP
#define SOFRE_RENDERCONTEXT_HPP

#include <sofre/object.hpp>
#include <sofre/window.hpp>
#include <sofre/program.hpp>
#include <sofre/scene.hpp>
#include <sofre/math.hpp>
#include <sofre/camera.hpp>

#include <memory>

namespace sofre {

class Renderer {
public:
    Renderer(const Window& desc, const Renderer* shared);
    ~Renderer();
    
    bool createSuccessfully() const { return m_creat_success; }
    bool shouldClose() const;

    void resize(int width, int height);
    void setBackgroundColor(float r, float g, float b, float a);
    void setCamera(const CameraParams& params);
    CameraParams& camera() { return m_camera; }
    const CameraParams& camera() const { return m_camera; }

    bool addShader(const Shader& shader) { return m_program.addShader(shader); }
    template<typename... Args>
    bool addShader(Args&&... args) {return addShader(Shader{std::forward<Args>(args)...});}
    bool addShaderFromFile(ShaderType type, const std::filesystem::path& path, bool isUTF8withoutBOM = false) {
        return m_program.addShaderFromFile(type, path, isUTF8withoutBOM);
    }
    bool buildProgram() {
        return m_program.build();
    }
    
    void addObject(const std::shared_ptr<Object>& obj);
    void removeObject(const std::shared_ptr<Object>& obj);
    
    void render(const Scene& scene);

private:
    CameraParams m_camera;
    mat4 m_view;
    mat4 m_proj;

    bool m_creat_success = false;
    Program m_program;
    Window m_windowDesc;

    struct Renderer_GL;
    Renderer_GL* gl = nullptr;
};

} // namespace sofre

#endif // SOFRE_RENDERCONTEXT_HPP
