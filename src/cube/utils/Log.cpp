#include "cube/utils/Log.hpp"

namespace cube {

    std::string Logger::levelToString(const LogLevel level) {
        switch (level) {
            case LogLevel::Info:    return "\033[32m INFO\033[0m";
            case LogLevel::Debug:   return "\033[34mDEBUG\033[0m";
            case LogLevel::Warning: return "\033[33m WARN\033[0m";
            case LogLevel::Error:   return "\033[31mERROR\033[0m";
            default: return "LOG";
        }
    }

}
