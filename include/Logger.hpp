#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <chrono>
#include <fmt/printf.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

enum class LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

class Logger {
private:
  std::ofstream logFile;
  static Logger *instance;
  static std::mutex logMutex;

  Logger() {}

  static std::string levelToString(LogLevel level) {
    switch (level) {
    case LogLevel::DEBUG:
      return "DEBUG";
    case LogLevel::INFO:
      return "INFO";
    case LogLevel::WARNING:
      return "WARNING";
    case LogLevel::ERROR:
      return "ERROR";
    case LogLevel::CRITICAL:
      return "CRITICAL";
    default:
      return "UNKNOWN";
    }
  }

public:
  static Logger &getInstance() {
    static Logger instance; // Thread-safe singleton
    return instance;
  }

  void setLogFile(const std::string &filename) {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open()) {
      logFile.close();
    }
    logFile.open(filename, std::ios::app);
    if (!logFile) {
      std::cerr << "Error opening log file: " << filename << std::endl;
    }
  }

  template <typename... Args>
  void log(LogLevel level, const char *format, Args &&...args) {
    std::lock_guard<std::mutex> lock(logMutex);

    std::string formattedMessage =
        fmt::sprintf(format, std::forward<Args>(args)...);

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm timeinfo = *std::localtime(&now_c);

    std::ostringstream logEntry;
    logEntry << "[" << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S") << "] "
             << levelToString(level) << ": " << formattedMessage << "\n";

    std::cout << logEntry.str();

    if (logFile.is_open()) {
      logFile << logEntry.str();
      logFile.flush();
    }
  }
};

// Macro for simplified usage
#define LOG(level, fmt, ...)                                                   \
  Logger::getInstance().log(LogLevel::level, fmt, ##__VA_ARGS__)

#endif // LOGGER_HPP
