#include <iostream>
#include <sstream>
#include <stdint.h>
#include <string>

enum class LogLevel { DEBUG, INFO, WARN, ERROR };

class Logger;

class LoggerPlant {
private:
  std::string log_;
  int32_t func_depth_;
  LogLevel current_level_;

  LoggerPlant(LogLevel level);
  ~LoggerPlant();

public:
  static auto getInstance() -> LoggerPlant &;
  void setLevel(LogLevel level);
  void output();
  void log(const std::string &message);
  auto enterFunc(const std::string &function_name) -> Logger;
  void exitFunc(const std::string &function_name);
};

class Logger {
private:
  std::string func_name_;
  std::string log_;
  LoggerPlant *plant_;
  LogLevel log_level_;

public:
  Logger(const std::string &func_name, LoggerPlant *plant_, LogLevel level);
  ~Logger();
  void log(LogLevel level, const std::string &message);
};