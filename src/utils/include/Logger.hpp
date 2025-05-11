#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <fmt/core.h>

namespace logger
{
    class Logger
    {
    public:
        enum eLevel
        {
            INFO,
            WARNING,
            ERROR,
            DEBUG
        };

        void           SetLogFile(const std::string& filename);
        static Logger& GetInstance();
        void           Log(const std::string& message, eLevel level = eLevel::INFO);

        template <typename... Args>
        void LogWithLocation(eLevel level, const char* file, int line, fmt::format_string<Args...> fmtStr, Args&&... args)
        {
            const char* baseFile = std::strrchr(file, '/');
            if (!baseFile) baseFile = std::strrchr(file, '\\');
            baseFile = baseFile ? baseFile + 1 : file;

            std::string formattedMsg = fmt::format(fmtStr, std::forward<Args>(args)...);
            std::string fullMsg = fmt::format("[{}:{}] {}", baseFile, line, formattedMsg);
            Log(fullMsg, level);
        }

    private:
        Logger();
        std::ofstream m_logFile;
        std::mutex    m_logMutex;
        std::string   GetTimestamp();
        std::string   LevelToString(eLevel level);
    };
} // namespace logger
