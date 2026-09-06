
#ifndef SOFRE_LOG_HPP
#define SOFRE_LOG_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <memory>
#include <utility>

namespace sofre {

/**
 * Stream-based logger.
 *
 * A "log line" is one LogStream lifetime: the message is buffered and flushed
 * (with a single prefix) when the LogStream is destroyed. This mirrors glutil's
 * stream-style logger so that prefixes line up one-per-log-line.
 *
 * Sinks are std::shared_ptr<std::ostream> so the underlying stream is directly
 * usable (e.g. to pass to third-party APIs) and so a custom formatter/streambuf
 * can be attached to the stream. Shared ownership prevents dangling when a
 * user-provided stream (e.g. a local std::ofstream) goes out of scope: the
 * logger keeps it alive. The default sinks (std::cout/std::cerr) use a
 * non-owning deleter so they are never destroyed.
 */
class Log {
public:
    using Sink = std::shared_ptr<std::ostream>;

    /**
     * RAII log stream. Buffers the message and flushes it (with prefix) on
     * destruction. Use via Log::info()/warn()/err() or the SOFRE_LOG_* macros.
     */
    class LogStream {
    public:
        LogStream(Sink sink, const char* level, bool enabled)
            : m_sink(std::move(sink)), m_level(level), m_enabled(enabled) {}

        LogStream(LogStream&& other) noexcept
            : m_sink(std::move(other.m_sink)),
              m_level(other.m_level),
              m_enabled(other.m_enabled),
              m_buffer(std::move(other.m_buffer)) {
            other.m_enabled = false; // moved-from stream must not flush
        }

        LogStream(const LogStream&) = delete;
        LogStream& operator=(const LogStream&) = delete;
        LogStream& operator=(LogStream&&) = delete;

        ~LogStream() {
            if (!m_enabled || !m_sink) return;
            auto msg = m_buffer.str();
            if (msg.empty()) return;
            // Strip a single trailing newline; the sink adds its own line ending.
            if (msg.back() == '\n') msg.pop_back();
            (*m_sink) << '[' << m_level << "] " << msg << std::endl;
        }

        template <typename T>
        LogStream& operator<<(T&& value) {
            if (m_enabled) m_buffer << std::forward<T>(value);
            return *this;
        }

        // Support std::endl / std::flush / other manipulators.
        LogStream& operator<<(std::ostream& (*manip)(std::ostream&)) {
            if (m_enabled) m_buffer << manip;
            return *this;
        }

    private:
        Sink m_sink;
        const char* m_level;
        bool m_enabled;
        std::ostringstream m_buffer;
    };

    /**
     * Initialize sofre's logging. Must be called once before any logging is used.
     */
    static void init();

    /**
     * Set the error sink. Takes ownership of the stream (keeps it alive).
     * To use a non-owned stream (e.g. std::cerr), pass a shared_ptr with a
     * no-op deleter via setErrorLoggerNonOwning().
     */
    static inline void setErrorLogger(Sink errlogger) {
        if (errlogger) errLogger() = std::move(errlogger);
    }
    static inline void setWarningLogger(Sink warnlogger) {
        if (warnlogger) warnLogger() = std::move(warnlogger);
    }
    static inline void setLogger(Sink logger) {
        if (logger) outLogger() = std::move(logger);
    }

    /**
     * Set a sink from a raw std::ostream* WITHOUT taking ownership.
     * The caller guarantees the stream outlives the logger. Use this for
     * std::cout/std::cerr or other long-lived streams.
     */
    static inline void setErrorLoggerPtr(std::ostream* s) {
        if (s) errLogger() = Sink(s, [](std::ostream*) {});
    }
    static inline void setWarningLoggerPtr(std::ostream* s) {
        if (s) warnLogger() = Sink(s, [](std::ostream*) {});
    }
    static inline void setLoggerPtr(std::ostream* s) {
        if (s) outLogger() = Sink(s, [](std::ostream*) {});
    }

    /** Begin an info-level log line. */
    static LogStream info() { return LogStream(outLogger(), "INFO", enabled()); }
    /** Begin a warning-level log line. */
    static LogStream warn() { return LogStream(warnLogger(), "WARNING", enabled()); }
    /** Begin an error-level log line. */
    static LogStream err() { return LogStream(errLogger(), "ERROR", enabled()); }

    /**
     * Returns a std::ostream that routes its output through sofre's logging
     * (the "info" sink). Useful for passing to third-party APIs that write to
     * a std::ostream (e.g. glutil::debug::printRuntimeInfo).
     */
    static std::ostream& outStream();
    /**
     * Returns a std::ostream that routes its output through sofre's error sink.
     */
    static std::ostream& errStream();

    /** Global enable/disable toggle. */
    static inline void enable(bool v) { enabled() = v; }
    static inline bool isEnabled() { return enabled(); }

private:
    // Function-local statics (Meyers singletons): constructed on first use and
    // destroyed in reverse order of construction. This guarantees the glutil
    // adapter streams (constructed later, inside Log::init) are destroyed
    // BEFORE these sinks, so the adapter's final flush never calls into a
    // destroyed logger.
    static Sink& outLogger() {
        static Sink l = Sink(&std::cout, [](std::ostream*) {});
        return l;
    }
    static Sink& warnLogger() {
        static Sink l = Sink(&std::cout, [](std::ostream*) {});
        return l;
    }
    static Sink& errLogger() {
        static Sink l = Sink(&std::cerr, [](std::ostream*) {});
        return l;
    }
    static bool& enabled() {
        static bool e = true;
        return e;
    }
};

} // namespace sofre

// TODO_easy : use SOFRE_DEBUG. maybe export gdm::defs as public linked target.
#ifndef SOFRE_LOG_ENABLED
#define SOFRE_LOG_ENABLED 1
#endif

#if SOFRE_LOG_ENABLED
#define SOFRE_LOG_INFO() ::sofre::Log::info()
#define SOFRE_LOG_WARN() ::sofre::Log::warn()
#define SOFRE_LOG_ERR()  ::sofre::Log::err()
#else
#define SOFRE_LOG_INFO() if constexpr (true) ; else ::sofre::Log::info()
#define SOFRE_LOG_WARN() if constexpr (true) ; else ::sofre::Log::warn()
#define SOFRE_LOG_ERR()  if constexpr (true) ; else ::sofre::Log::err()
#endif

#endif // SOFRE_LOG_HPP
