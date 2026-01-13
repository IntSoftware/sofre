#ifndef SOFRE_RENDERCONTEXT_HPP
#define SOFRE_RENDERCONTEXT_HPP

struct GLFWwindow;

namespace sofre {

struct Window;

class Renderer {
public:
    Renderer(const Window& desc, const Renderer* shared);
    ~Renderer();

    void render();
    bool shouldClose() const;
    bool createSuccessfully() const { return m_creat_success; }
private:
    GLFWwindow* m_window = nullptr;
    bool m_creat_success = false;
};

} // namespace sofre

#endif // SOFRE_RENDERCONTEXT_HPP
