//
// Created by UshioHayase on 2025-11-11.
//

#include "logger.hpp"

#include <Windows.h>

#include <chrono>
#include <ctime>
#include <format>

Logger::Logger() : minLevel(LogLevel::Info)
{
    const auto now = std::chrono::system_clock::now();
    const auto timeT = std::chrono::system_clock::to_time_t(now);

    tm localTime{};
    localtime_s(&localTime, &timeT);

    const std::wstring fileName =
        std::format(L"visual-algorithm-core-log-{}.{}.{}-{}:{}:{}.txt", localTime.tm_year + 1900, localTime.tm_mon + 1,
                    localTime.tm_mday, localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
    fileHandle = CreateFileW(fileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL, nullptr);

    consoleOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    consoleErrHandle = GetStdHandle(STD_ERROR_HANDLE);
}

Logger::~Logger()
{
    if (GetHandleInformation(fileHandle, nullptr) != 0)
        CloseHandle(fileHandle);
}

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::setLogLevel(const LogLevel lvl)
{
    minLevel = lvl;
}

void setLogLevel(const LogLevel lvl)
{
    Logger::getInstance().setLogLevel(lvl);
}

std::string Logger::formatMessage(const std::string& format) const
{
    return format;
}

std::string Logger::buildLogEntry(LogLevel lvl, const char* file, int line, const char* func,
                                  const std::string& format) const
{
    std::stringstream ss;

    // 1. 현재 시간 구하기 (std::chrono)
    const auto now = std::chrono::system_clock::now();
    const auto timeT = std::chrono::system_clock::to_time_t(now);

    tm localTime;
    localtime_s(&localTime, &timeT);

    // 시간 포맷팅: [YYYY-MM-DD HH:MM:SS]
    ss << "[" << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << "]";

    // 로그 레벨 표기
    switch (lvl)
    {
    case LogLevel::Info:
        ss << " [INFO] ";
        break;
    case LogLevel::Warning:
        ss << " [WARN] ";
        break;
    case LogLevel::Error:
        ss << " [ERROR]  ";
        break;
    }

    // 파일, 줄, 함수 정보 표기
    ss << "[" << file << ":" << line << ", " << func << "] ";

    ss << format;

    return ss.str();
}

void Logger::outputToChannels(const std::string& log) const
{
    if (fileHandle != nullptr)
        WriteFile(fileHandle, log.c_str(), log.length(), nullptr, nullptr);

    constexpr char nextLine = '\n';
    if (consoleOutHandle == nullptr)
        return;
    WriteConsole(consoleOutHandle, log.c_str(), log.length(), nullptr, nullptr);
    WriteConsole(consoleOutHandle, &nextLine, 1, nullptr, nullptr);
}
