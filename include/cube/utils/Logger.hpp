#ifndef CUBE_LOGGER_HPP
#define CUBE_LOGGER_HPP

#include <string_view>
#include <format>

namespace cube {

    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    class Log {
    public:
        static void set_level(LogLevel level);

        template <typename... Args>
        static void debug(std::format_string<Args...> fmt, Args&&... args) {
            log(LogLevel::Debug, fmt.get(), std::make_format_args(args...));
        }

        template <typename... Args>
        static void info(std::format_string<Args...> fmt, Args&&... args) {
            log(LogLevel::Info, fmt.get(), std::make_format_args(args...));
        }

        template <typename... Args>
        static void warn(std::format_string<Args...> fmt, Args&&... args) {
            log(LogLevel::Warning, fmt.get(), std::make_format_args(args...));
        }

        template <typename... Args>
        static void error(std::format_string<Args...> fmt, Args&&... args) {
            log(LogLevel::Error, fmt.get(), std::make_format_args(args...));
        }

    private:
        static void log(LogLevel level, std::string_view fmt, std::format_args args);

        static inline auto m_min_level = LogLevel::Info;
    };
}

#endif //CUBE_LOGGER_HPP