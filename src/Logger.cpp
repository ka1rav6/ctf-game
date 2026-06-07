#include "../include/Logger.h"

const char* Logger::levelString(LogLevel level)
{
    switch(level)
    {
        case LogLevel::TRACE: return "TRACE";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
    }

    return "UNKNOWN";
}

const char* Logger::levelColor(LogLevel level)
{
    switch(level)
    {
        case LogLevel::TRACE: return "\033[36m"; // cyan
        case LogLevel::INFO:  return "\033[32m"; // green
        case LogLevel::WARN:  return "\033[33m"; // yellow
        case LogLevel::ERROR: return "\033[31m"; // red
        case LogLevel::FATAL: return "\033[35m"; // magenta
    }

    return "\033[0m";
}

void Logger::log(
    LogLevel level,
    const std::string& message,
    const char* file,
    int line
)
{
    std::ostream& out =
        (level == LogLevel::ERROR || level == LogLevel::FATAL)
            ? std::cerr
            : std::cout;

    out
        << levelColor(level)
        << "[" << levelString(level) << "] "
        << file << ":" << line
        << " -> "
        << message
        << "\033[0m"
        << std::endl;

    if(level == LogLevel::FATAL)
        std::terminate();
}