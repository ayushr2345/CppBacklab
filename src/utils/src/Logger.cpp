#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include "Logger.hpp"

namespace logger
{
    Logger::Logger() {}

    void Logger::SetLogFile(const std::string& filename)
    {
        std::lock_guard<std::mutex> lock(m_logMutex);
        m_logFile.open(filename, std::ios::trunc);
    }

    Logger& Logger::GetInstance()
    {
        static Logger instance;
        return instance;
    }

    void Logger::Log(const std::string& message, eLevel level)
    {
        std::lock_guard<std::mutex> lock(m_logMutex);
        std::string time = GetTimestamp();
        std::string levelStr = LevelToString(level);
        std::string fullMessage = "[" + time + "][" + levelStr + "] " + message;

        if (m_logFile.is_open())
        {
            m_logFile << fullMessage << std::endl;
        }
        else
        {
            std::cout << fullMessage << std::endl;
        }
    }

    std::string Logger::GetTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm buf;
#ifdef _WIN32
        localtime_s(&buf, &in_time_t);
#else
        localtime_r(&in_time_t, &buf);
#endif
        std::ostringstream ss;
        ss << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    std::string Logger::LevelToString(eLevel level)
    {
        switch (level)
        {
        case eLevel::INFO:
            return "INFO";
        case eLevel::WARNING:
            return "WARNING";
        case eLevel::ERROR:
            return "ERROR";
        case eLevel::DEBUG:
            return "DEBUG";
        default:
            return "UNKNOWN";
        }
    }
} // namespace logger
