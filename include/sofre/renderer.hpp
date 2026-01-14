#ifndef SOFRE_RENDERCONTEXT_HPP
#define SOFRE_RENDERCONTEXT_HPP

#include <sofre/object.hpp>

#include <memory>
#include <list>

namespace sofre {
struct Window;

class Renderer {
public:
    Renderer(const Window& desc, const Renderer* shared);
    ~Renderer();
    
    bool createSuccessfully() const { return m_creat_success; }
    bool shouldClose() const;
    
    void addObject(const std::shared_ptr<Object>& obj);
    void removeObject(const std::shared_ptr<Object>& obj);
    
    void render();
    
private:
    bool m_creat_success = false;
    std::list<std::shared_ptr<Object>> objectList;
    
    struct Renderer_GL;
    Renderer_GL* gl = nullptr;
};

} // namespace sofre

#endif // SOFRE_RENDERCONTEXT_HPP
