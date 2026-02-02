#ifndef SOFRE_OS_DETECT_HPP
#define SOFRE_OS_DETECT_HPP

#if defined(_WIN32) || defined(WIN32)
/* -------- Windows -------- */
#define SOFRE_OS_WINDOWS 1
#define SOFRE_OS_LINUX 0
#define SOFRE_OS_MACOS 0

#elif defined(__APPLE__) && defined(__MACH__)
/* -------- macOS -------- */
#define SOFRE_OS_WINDOWS 0
#define SOFRE_OS_LINUX 0
#define SOFRE_OS_MACOS 1

#elif defined(__linux__)
/* -------- Linux -------- */
#define SOFRE_OS_WINDOWS 0
#define SOFRE_OS_LINUX 1
#define SOFRE_OS_MACOS 0

#else
#define SOFRE_OS_WINDOWS 0
#define SOFRE_OS_LINUX 0
#define SOFRE_OS_MACOS 0
#endif

namespace sofre {
constexpr bool IsWindows = SOFRE_OS_WINDOWS;
constexpr bool IsLinux = SOFRE_OS_LINUX;
constexpr bool IsMacOS = SOFRE_OS_MACOS;
} // namespace sofre

#endif // !SOFRE_OS_DETECT_HPP