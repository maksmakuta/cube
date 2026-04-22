#include <cube/utils/Logger.hpp>

#include <chrono>
#include <print>

namespace cube {

    void Log::set_level(const LogLevel level) {
        m_min_level = level;
    }

    struct LogStyle {
        std::string_view label;
        std::string_view color;
    };

    LogStyle get_style(const LogLevel level) {
        switch (level) {
            case LogLevel::Debug:    return {"DEBUG", "\033[36m"};
            case LogLevel::Info:     return {"INFO",  "\033[32m"};
            case LogLevel::Warning:  return {"WARN",  "\033[33m"};
            case LogLevel::Error:    return {"ERROR", "\033[31m"};
            case LogLevel::Critical: return {"CRIT",  "\033[1;31m"};
            default:                 return {"LOG",   "\033[0m"};
        }
    }

    void Log::log(const LogLevel level, const std::string_view fmt, const std::format_args args) {
        if (level < m_min_level) return;

        const std::chrono::zoned_time cur_time{
            std::chrono::current_zone(),
            std::chrono::system_clock::now()
        };

        const auto [label, color] = get_style(level);
        const std::string reset = "\033[0m";
        const std::string gray  = "\033[90m";

        std::string message = std::vformat(fmt, args);

        std::println("{}[{:%H:%M:%S}]{} {}[{:<7}]{} {}",
                     gray, cur_time, reset,
                     color, label, reset,
                     message);
    }

}