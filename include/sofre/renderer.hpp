#ifndef SOFRE_RENDERCONTEXT_HPP
#define SOFRE_RENDERCONTEXT_HPP

#include <sofre/object.hpp>
#include <sofre/window.hpp>
#include <sofre/program.hpp>
#include <sofre/scene.hpp>

#include <memory>

namespace sofre {

class Renderer {
public:
    Renderer(const Window& desc, const Renderer* shared);
    ~Renderer();
    
    bool createSuccessfully() const { return m_creat_success; }
    bool shouldClose() const;

    void setBackgroundColor(float r, float g, float b, float a);

    bool addShader(const Shader& shader) {
        return m_program.addShader(shader);
    }
    bool buildProgram() {
        return m_program.build();
    }
    
    void addObject(const std::shared_ptr<Object>& obj);
    void removeObject(const std::shared_ptr<Object>& obj);
    
    void render(const Scene& scene);

private:
    bool m_creat_success = false;
    Program m_program;
    
    struct Renderer_GL;
    Renderer_GL* gl = nullptr;
};

} // namespace sofre

#endif // SOFRE_RENDERCONTEXT_HPP
