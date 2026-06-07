#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <sstream>
#include <string>

enum class LogLevel {
    TRACE,
    INFO,
    WARN,
    ERROR,
    FATAL
};

class Logger {
public:
    static void log(
        LogLevel level,
        const std::string& message,
        const char* file,
        int line
    );

private:
    static const char* levelString(LogLevel level);
    static const char* levelColor(LogLevel level);
};

#define LOG_TRACE(msg) Logger::log(LogLevel::TRACE, msg, __FILE__, __LINE__)
#define LOG_INFO(msg)  Logger::log(LogLevel::INFO,  msg, __FILE__, __LINE__)
#define LOG_WARN(msg)  Logger::log(LogLevel::WARN,  msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) Logger::log(LogLevel::ERROR, msg, __FILE__, __LINE__)
#define LOG_FATAL(msg) Logger::log(LogLevel::FATAL, msg, __FILE__, __LINE__)

#endif