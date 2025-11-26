//
// Created by UshioHayase on 2025-11-11.
//

#include "Logger.hpp"

#include <Windows.h>

#include <chrono>
#include <ctime>
#include <format>

Logger::Logger() : min_level(LogLevel::Info)
{
    const auto now = std::chrono::system_clock::now();
    const auto time_t = std::chrono::system_clock::to_time_t(now);

    tm local_time{};
    localtime_s(&local_time, &time_t);

    const std::wstring file_name =
        std::format(L"visual-algorithm-core-log-{}.{}.{}-{}:{}:{}.txt", local_time.tm_year + 1900, local_time.tm_mon + 1,
                    local_time.tm_mday, local_time.tm_hour, local_time.tm_min, local_time.tm_sec);
    file_handle = CreateFileW(file_name.c_str(), GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL, nullptr);
#ifdef _DEBUG
    console_out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    console_err_handle = GetStdHandle(STD_ERROR_HANDLE);
#endif
}

Logger::~Logger()
{
    if (GetHandleInformation(file_handle, nullptr) != 0)
        CloseHandle(file_handle);
}

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::setLogLevel(LogLevel lvl)
{
    min_level = lvl;
}

void setLogLevel(LogLevel lvl)
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
    const auto time_t = std::chrono::system_clock::to_time_t(now);

    tm local_time;
    localtime_s(&local_time, &time_t);

    // 시간 포맷팅: [YYYY-MM-DD HH:MM:SS]
    ss << "[" << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S") << "]";

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
    if (file_handle != nullptr)
        WriteFile(file_handle, log.c_str(), log.length(), nullptr, nullptr);

    constexpr char NEXT_LINE = '\n';
    if (console_out_handle == nullptr)
        return;
#ifdef _DEBUG
    WriteConsole(console_out_handle, log.c_str(), log.length(), nullptr, nullptr);
    WriteConsole(console_out_handle, &NEXT_LINE, 1, nullptr, nullptr);
#endif
}
