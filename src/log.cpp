#include "os_detect.hpp"

#include <sofre/log.hpp>

#include <glutil/logging.hpp>
#include <glutil/debug_stacktrace.hpp>

#if SOFRE_OS_WINDOWS
#include <windows.h>
#endif

#include <iostream>
#include <ostream>
#include <string>

namespace sofre {

void Log::init() {
    // The initial loggers are std::cout/std::cerr, so set the Windows console
    // to UTF-8 before any logging happens.
#if SOFRE_OS_WINDOWS
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // Disable glutil's own "[LEVEL]" prefix; sofre adds its own prefix.
    glutil::enablePrefixAllLoggers(false);

    // Point glutil's loggers directly at sofre's sink streams.
    glutil::Logger::outLogger().setOutput(outLogger().get());
    glutil::Logger::errLogger().setOutput(errLogger().get());
}

std::ostream& Log::outStream() {
    return *outLogger();
}

std::ostream& Log::errStream() {
    return *errLogger();
}

}