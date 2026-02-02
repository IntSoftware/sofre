#include "os_detect.hpp"

#include <sofre/log.hpp>

#include <sstream>
#include <iostream>

#ifdef SOFRE_OS_WINDOWS
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#elif defined(SOFRE_OS_LINUX) || defined(SOFRE_OS_MACOS)
#include <execinfo.h>
#endif

#define SOFRE_STACK_DEPTH 64


namespace sofre {

std::string getStackTrace() {
    std::stringstream stackTrace;

#ifdef SOFRE_OS_WINDOWS
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
#elif defined(SOFRE_OS_LINUX) || defined(SOFRE_OS_MACOS)
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
}