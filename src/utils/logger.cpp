#include "logger.hpp"

Logger::Logger(LogLevel level) : function_depth_(0), current_level_(level) {}

Logger::~Logger() = default;

auto Logger::getInstance() -> Logger & {
  static Logger logger(LogLevel::INFO);
  return logger;
}

void Logger::setLevel(LogLevel level) { current_level_ = level; }

void Logger::log(LogLevel level, const std::string &message) {
  if (level >= current_level_) {
    for (int32_t i = 0; i < function_depth_; ++i) {
      log_stream_ << "  ";
    }
    switch (level) {
    case LogLevel::DEBUG:
      log_stream_ << "[DEBUG] ";
      break;
    case LogLevel::INFO:
      log_stream_ << "[INFO] ";
      break;
    case LogLevel::WARN:
      log_stream_ << "[WARN] ";
      break;
    case LogLevel::ERROR:
      log_stream_ << "[ERROR] ";
      break;
    }
    log_stream_ << message << '\n';
  }
}

void Logger::output() {
  /*
  std::cout << log_stream_.str();
  log_stream_.clear();
  */
}

void Logger::enterFunction(const std::string &function_name) {
  for (int32_t i = 0; i < function_depth_; ++i) {
    log_stream_ << "  ";
  }
  log_stream_ << "Entering function: " << function_name << '\n';
  ++function_depth_;
}

void Logger::exitFunction() {
  if (function_depth_ > 0) {
    --function_depth_;
  }
}