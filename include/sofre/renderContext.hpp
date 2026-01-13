#ifndef SOFRE_RENDERCONTEXT_HPP
#define SOFRE_RENDERCONTEXT_HPP

namespace sofre {

struct Window;

class RenderContext {
public:
    RenderContext();
    ~RenderContext();

    void render();
    bool shouldClose() const;

private:
    friend class GraphicEngine;

    bool create(const Window& desc, RenderContext* shared);

private:
    struct Impl;
    Impl* m_impl = nullptr;
};

} // namespace sofre

#endif // SOFRE_RENDERCONTEXT_HPP
