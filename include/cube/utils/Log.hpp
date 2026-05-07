#ifndef CUBE_LOG_HPP
#define CUBE_LOG_HPP

#include <format>
#include <chrono>
#include <print>

namespace cube {

    enum class LogLevel { Info, Debug, Warning, Error };

    class Logger {
    public:
        template<typename... Args>
        static void log(const LogLevel level, std::format_string<Args...> fmt, Args&&... args) {
            const auto now = std::chrono::system_clock::now();
            auto local_now = std::chrono::current_zone()->to_local(now);
            std::string s = std::format(fmt, std::forward<Args>(args)...);
            std::println("[{:%H:%M:%S}] [{}] {}", local_now, levelToString(level), s);
        }

    private:
        static std::string levelToString(LogLevel level);
    };

    template<typename... Args>
    void info(std::format_string<Args...> fmt, Args&&... args) {
        Logger::log(LogLevel::Info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void debug(std::format_string<Args...> fmt, Args&&... args) {
        Logger::log(LogLevel::Debug, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warn(std::format_string<Args...> fmt, Args&&... args){
        Logger::log(LogLevel::Warning, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error(std::format_string<Args...> fmt, Args&&... args) {
        Logger::log(LogLevel::Error, fmt, std::forward<Args>(args)...);
    }

}

#endif //CUBE_LOG_HPP
