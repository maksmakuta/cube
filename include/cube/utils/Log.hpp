#ifndef CUBE_LOG_HPP
#define CUBE_LOG_HPP

#include <format>
#include <iostream>

namespace cube {

    enum class Level : unsigned char {
        Trace, Debug, Info, Warn, Error, Fatal, Off
    };

    static auto MIN_LEVEL = Level::Trace;

    constexpr std::string_view level_str(const Level lvl) {
        switch (lvl) {
            case Level::Trace: return "TRACE";
            case Level::Debug: return "DEBUG";
            case Level::Info:  return "INFO ";
            case Level::Warn:  return "WARN ";
            case Level::Error: return "ERROR";
            case Level::Fatal: return "FATAL";
            default: return "OFF  ";
        }
    }

    constexpr std::string_view level_color(const Level lvl) {
        switch (lvl) {
            case Level::Trace: return "\033[37m";
            case Level::Debug: return "\033[36m";
            case Level::Info:  return "\033[32m";
            case Level::Warn:  return "\033[33m";
            case Level::Error: return "\033[31m";
            case Level::Fatal: return "\033[41m";
            default: return "\033[0m";
        }
    }

    template <typename... Args>
    void log(
        Level lvl,
        std::format_string<Args...> fmt,
        Args&&... args
    ) {
        if (static_cast<int>(lvl) < static_cast<int>(MIN_LEVEL))
            return;

        auto msg = std::format(fmt, std::forward<Args>(args)...);

        std::cout << level_color(lvl)
                  << "[" << level_str(lvl) << "] "
                  << " → " << msg
                  << "\033[0m\n";
    }

    template <typename... Args>
    void trace(std::format_string<Args...> f, Args&&... a) {
        log(Level::Trace, f, std::forward<Args>(a)...);
    }

    template <typename... Args>
    void debug(std::format_string<Args...> f, Args&&... a) {
        log(Level::Debug, f, std::forward<Args>(a)...);
    }

    template <typename... Args>
    void info(std::format_string<Args...> f, Args&&... a) {
        log(Level::Info, f, std::forward<Args>(a)...);
    }

    template <typename... Args>
    void warn(std::format_string<Args...> f, Args&&... a) {
        log(Level::Warn, f, std::forward<Args>(a)...);
    }

    template <typename... Args>
    void error(std::format_string<Args...> f, Args&&... a) {
        log(Level::Error, f, std::forward<Args>(a)...);
    }

    template <typename... Args>
    void fatal(std::format_string<Args...> f, Args&&... a) {
        log(Level::Fatal, f, std::forward<Args>(a)...);
    }

}

#endif //CUBE_LOG_HPP
