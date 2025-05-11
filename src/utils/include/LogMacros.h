#pragma once
#include "Logger.hpp"

#define LOG_DEBUG(fmt, ...) logger::Logger::GetInstance().LogWithLocation(logger::Logger::eLevel::DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  logger::Logger::GetInstance().LogWithLocation(logger::Logger::eLevel::INFO,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  logger::Logger::GetInstance().LogWithLocation(logger::Logger::eLevel::WARNING, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) logger::Logger::GetInstance().LogWithLocation(logger::Logger::eLevel::ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
