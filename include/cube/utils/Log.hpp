#ifndef CUBE_LOG_HPP
#define CUBE_LOG_HPP

#include <type_traits>
#include <format>
#include <print>
#include <source_location>
#include <chrono>
#include <string>
#include <string_view>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>

namespace cube {
    enum class LogLevel { Debug, Info, Warning, Error, Critical };

    template<typename... Args>
    struct LogFormat {
        std::format_string<Args...> fmt;
        std::source_location loc;

        template<typename String>
        consteval LogFormat(const String &str, std::source_location loc = std::source_location::current())
            : fmt(str), loc(loc) {
        }
    };

    class Log final {
    public:
        static void init() {
            running_ = true;
            worker_ = std::jthread(worker_loop);
        }

        static void shutdown() {
            running_ = false;
            cv_.notify_one();
            if (worker_.joinable()) {
                worker_.join();
            }
        }

        template<typename... Args>
        static void d(LogFormat<std::type_identity_t<Args>...> fmt, Args &&... args) {
            log(LogLevel::Debug, fmt.loc, fmt.fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void i(LogFormat<std::type_identity_t<Args>...> fmt, Args &&... args) {
            log(LogLevel::Info, fmt.loc, fmt.fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void w(LogFormat<std::type_identity_t<Args>...> fmt, Args &&... args) {
            log(LogLevel::Warning, fmt.loc, fmt.fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void e(LogFormat<std::type_identity_t<Args>...> fmt, Args &&... args) {
            log(LogLevel::Error, fmt.loc, fmt.fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void c(LogFormat<std::type_identity_t<Args>...> fmt, Args &&... args) {
            log(LogLevel::Critical, fmt.loc, fmt.fmt, std::forward<Args>(args)...);
        }

    private:
        static constexpr std::string_view level_string(const LogLevel level) {
            switch (level) {
                case LogLevel::Debug:       return "DEBUG";
                case LogLevel::Info:        return "INFO ";
                case LogLevel::Warning:     return "WARN ";
                case LogLevel::Error:       return "ERROR";
                case LogLevel::Critical:    return "CRIT ";
            }
            return "   ?";
        }

        static constexpr std::string_view file_name_only(const std::string_view path) {
            const auto pos = path.find_last_of("/\\");
            return pos == std::string_view::npos ? path : path.substr(pos + 1);
        }

        static void worker_loop() {
            std::vector<std::string> local_buffer;
            while (running_ || !buffer_.empty()) {
                {
                    std::unique_lock<std::mutex> lock(mutex_);
                    cv_.wait(lock, [] { return !buffer_.empty() || !running_; });

                    local_buffer.swap(buffer_);
                }

                for (const auto &msg: local_buffer) {
                    std::print("{}", msg);
                }
                local_buffer.clear();
            }
        }

        template<typename... Args>
        static void log(const LogLevel level, const std::source_location &loc, std::format_string<Args...> fmt,
                        Args &&... args) {
            {
                std::string user_msg = std::format(fmt, std::forward<Args>(args)...);
                auto now = std::chrono::system_clock::now();
                std::string full_msg = std::format(
                    "[{:%Y-%m-%d %H:%M:%OS}] [{}] [{}:{}] {}\n",
                    now,
                    level_string(level),
                    file_name_only(loc.file_name()),
                    loc.line(),
                    user_msg
                );
                std::lock_guard lock(mutex_);
                buffer_.push_back(std::move(full_msg));
            }
            cv_.notify_one();
        }

        static inline std::vector<std::string> buffer_;
        static inline std::mutex mutex_;
        static inline std::condition_variable cv_;
        static inline std::jthread worker_;
        static inline std::atomic<bool> running_{false};
    };
}

#endif //CUBE_LOG_HPP
