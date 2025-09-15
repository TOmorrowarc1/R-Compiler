#include <iostream>
#include <sstream>
#include <stdint.h>
#include <string>

enum class LogLevel { DEBUG, INFO, WARN, ERROR };

class Logger {
private:
  std::stringstream log_stream_;
  int32_t function_depth_;
  LogLevel current_level_;

  Logger(LogLevel level);
  ~Logger();

public:
  static auto getInstance() -> Logger &;
  void setLevel(LogLevel level);
  void log(LogLevel level, const std::string &message);
  void output();
  void enterFunction(const std::string &function_name);
  void exitFunction();
};