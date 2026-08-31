#include "os_detect.hpp"

#include <sofre/log.hpp>

#include <glutil/logging.hpp>

#include <sstream>
#include <iostream>
#include <streambuf>
#include <ostream>
#include <string>

#if SOFRE_OS_WINDOWS
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#elif SOFRE_OS_LINUX || SOFRE_OS_MACOS
#include <execinfo.h>
#endif

#define SOFRE_STACK_DEPTH 64


namespace sofre {

std::string getStackTrace() {
    std::stringstream stackTrace;

#if SOFRE_OS_WINDOWS
    // Windows: Stack trace with CaptureStackBackTrace
    void* stack[SOFRE_STACK_DEPTH];
    unsigned short frames = CaptureStackBackTrace(0, SOFRE_STACK_DEPTH, stack, nullptr);
    SymInitialize(GetCurrentProcess(), nullptr, TRUE);
    for (unsigned short i = 0; i < frames; i++) {
        DWORD64 address = (DWORD64)(stack[i]);

        SYMBOL_INFO* symbol = new SYMBOL_INFO[sizeof(SYMBOL_INFO) + 256];
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = 255;

        // function name from location
        if (SymFromAddr(GetCurrentProcess(), address, 0, symbol)) {
            stackTrace << "    Frame " << i << ": " << symbol->Name << " (0x"
                      << std::hex << address << ")" << std::endl;
        } else {
            stackTrace << "    Frame " << i << ": (Unknown symbol) (0x" << std::hex
                      << address << ")" << std::endl;
        }

        delete[] symbol;
    }
    SymCleanup(GetCurrentProcess());
#elif SOFRE_OS_LINUX || SOFRE_OS_MACOS
    // Linux: Stack trace with backtrace()
    // TODO : check
    void* stack[SOFRE_STACK_DEPTH];
    int frameCount = backtrace(stack, SOFRE_STACK_DEPTH);
    char** frameStrings = backtrace_symbols(stack, frameCount);

    for (int i = 0; i < frameCount; i++) {
        stackTrace << "    Frame " << i << ": " << frameStrings[i] << std::endl;
    }
    free(frameStrings);
#else
    stackTrace << "    Unknown" << std::endl;
#endif

    return stackTrace.str();
}

namespace {
// GLUtil Logging Adapter redirects glutil's stream-style logger (which writes to a std::ostream)
// into sofre's function-style Log. They are not exposed in the public header.

/**
 * Custom std::streambuf that redirects glutil logs to sofre::Log.
 */
class SofreLogBuffer : public std::streambuf {
public:
    explicit SofreLogBuffer(bool isError = false) : isErrorBuffer(isError) {}

protected:
    int overflow(int ch) override {
        if (ch == EOF) return EOF;
        buffer += static_cast<char>(ch);
        return ch;
    }
    int sync() override {
        flush();
        return 0;
    }

private:
    std::string buffer;
    bool isErrorBuffer;

    void flush() {
        if (buffer.empty()) return;

        // glutil's prefix is disabled via enablePrefixAllLoggers(false), so the
        // buffered message is the raw log text. Route it by sink severity.
        (isErrorBuffer ? Log::error : Log::log)(buffer);
        buffer.clear();
    }
};

/**
 * Custom std::ostream that writes to sofre's logging system.
 */
class SofreLogStream : public std::ostream {
public:
    explicit SofreLogStream(bool isError = false) : std::ostream(&buffer), buffer(isError) {}
private:
    SofreLogBuffer buffer;
};

/**
 * Lazily-constructed adapter streams. Function-local statics guarantee:
 *  - construction on first use (after Log's sinks are already initialized),
 *  - destruction in reverse order of construction, i.e. BEFORE Log's sinks
 *    are destroyed, so the final flush never calls into a dead logger.
 */
SofreLogStream& stdoutAdapter() {
    static SofreLogStream s(false);  // false = not error
    return s;
}

SofreLogStream& stderrAdapter() {
    static SofreLogStream s(true);   // true = error
    return s;
}

} // namespace

void Log::init() {
    // Disable glutil's own "[LEVEL]" prefix; sofre adds its own prefix.
    glutil::enablePrefixAllLoggers(false);

    // Redirect glutil's loggers to sofre's logging.
    glutil::Logger::stdoutLogger().setOutput(&stdoutAdapter());
    glutil::Logger::stderrLogger().setOutput(&stderrAdapter());
}

}