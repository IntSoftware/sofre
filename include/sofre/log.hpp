
#ifndef SOFRE_LOG_HPP
#define SOFRE_LOG_HPP

#include <iostream>

namespace sofre {

class Log {
public:
    using Logger = std::function<void(std::string_view)>;
    
    static inline void setErrorLogger(Logger errlogger) {
        if(errlogger) log_err = std::move(errlogger);
    }
    static inline void setLogger(Logger logger) {
        if(logger) log_out = std::move(logger);
    }
    static inline void log(std::string_view message) { log_out(message); }
    static inline void error(std::string_view message) { log_err(message); }
    
    static inline void defaultLogConsumer(std::string_view msg) {
        std::cout << "[sofre] [info] " <<  msg << std::endl;
    }
    static inline void defaultErrConsumer(std::string_view msg) {
        std::cerr << "[sofre] [error] " << msg << std::endl;
    }
    static inline void noopLogConsumer(std::string_view) {}
    
private:
    static inline Logger log_out = Log::defaultLogConsumer;
    static inline Logger log_err = Log::defaultErrConsumer;
};

}
#endif
