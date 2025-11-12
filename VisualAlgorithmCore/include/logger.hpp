//
// Created by UshioHayase on 2025-11-11.
//

#ifndef VISUALALGORITHMCORE_LOGGER_H
#define VISUALALGORITHMCORE_LOGGER_H

#include <ios>
#include <sstream>
#include <string>

enum class LogLevel
{
    Info,
    Warning,
    Error
};

class Logger
{
  private:
    void* consoleOutHandle = nullptr;
    void* consoleErrHandle = nullptr;
    void* fileHandle = nullptr;
    LogLevel minLevel;

  public:
    static Logger& getInstance();

    void setLogLevel(const LogLevel lvl);

    // 외부에서 복사/이동을 금지
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    template <typename... Args>
    void write(LogLevel level, const char* file, int line, const char* function, const std::string& format,
               Args... args) const;

  private:
    Logger();
    ~Logger();

    // 재귀 끝
    std::string formatMessage(const std::string& format) const;
    // 재귀 호출
    template <typename Value, typename... Rest>
    std::string formatMessage(const std::string& format, Value&& first, Rest&&... rest) const;

    // 최종 로그 문자열을 만드는 함수
    std::string buildLogEntry(const LogLevel lvl, const char* file, int line, const char* func,
                              const std::string& format) const;

    // 각 채널에 출력하는 함수
    void outputToChannels(const std::string& log) const;
};

void setLogLevel(const LogLevel lvl);

template <typename Value, typename... Rest>
std::string Logger::formatMessage(const std::string& format, Value&& first, Rest&&... rest) const
{
    auto bracePos = format.find("{}");

    if (bracePos == std::string::npos)
        return format;

    std::string before = format.substr(0, bracePos);

    std::string after = format.substr(bracePos + 2);

    std::stringstream ss;

    if constexpr (std::is_convertible_v<Value, const char*>)
        ss << first;
    else if constexpr (std::is_same_v<std::decay_t<Value>, std::string>)
        ss << first;
    else if constexpr (std::is_pointer_v<std::decay_t<Value>>)
        ss << "0x" << std::hex << reinterpret_cast<uintptr_t>(first);
    else
        ss << first;

    return before + ss.str() + formatMessage(after, std::forward<Rest>(rest)...);
}

template <typename... Args>
void Logger::write(const LogLevel level, const char* file, int line, const char* function, const std::string& format,
                   Args... args) const
{
    if (level < minLevel)
        return;

    std::string userMessage = formatMessage(format, args...);
    std::string finalLog = buildLogEntry(level, file, line, function, userMessage);
    outputToChannels(finalLog);
}

#endif // VISUALALGORITHMCORE_LOGGER_H
