#include "Logger.hpp"

Logger *Logger::instance = nullptr;
std::mutex Logger::logMutex;
