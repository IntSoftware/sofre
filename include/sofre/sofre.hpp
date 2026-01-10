#ifndef SOFRE_HPP
#define SOFRE_HPP

#include <memory>
#include <functional>
#include <iostream>

#include <sofre/enums.hpp>

namespace sofre {

class Log {
public:
    static inline void defaultLogConsumer(std::string_view msg) {
        std::cout << "[sofre] [info]" <<  msg << std::endl;
    }
    static inline void defaultErrConsumer(std::string_view msg) {
        std::cerr << "[sofre] [error]" << msg << std::endl;
    }
    static inline void noopLogConsumer(std::string_view) {}
};


class System {
public:
    static int init();
    static void terminate();

    using Logger = std::function<void(std::string_view)>;

    static inline void setErrorLogger(Logger errlogger) {
        log_err = errlogger ? std::move(errlogger) : Log::defaultErrConsumer;
    }
    static inline void setLogger(Logger logger) {
        log_out = logger ? std::move(logger) : Log::defaultLogConsumer;
    }
    static inline void log(std::string_view message) { log_out(message); }
    static inline void error(std::string_view message) { log_err(message); }
    
private:
    static Logger log_out;
    static Logger log_err;
};



class GraphicEngine {
    // Engine lifecycle
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;

//    // Window management
//    virtual std::shared_ptr<Window> createWindow(const std::string &title,
//                                                 int width,
//                                                 int height) = 0;

//    virtual void destroyWindow(std::shared_ptr<Window> window) = 0;

    
    virtual void renderFrame() = 0;
    virtual void resize(int width, int height) = 0;

//    virtual void addObject(std::shared_ptr<RenderObject> object) = 0;
//    virtual void removeObject(std::shared_ptr<RenderObject> object) = 0;
    
    void setCursorMode(CursorMode mode);
    void setStickyKey(bool sticky);
};

std::unique_ptr<GraphicEngine> createEngine();

} // namespace sofre

#endif
