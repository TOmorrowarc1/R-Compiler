#include "logger.hpp"

LoggerPlant::LoggerPlant(LogLevel level)
    : func_depth_(0), current_level_(level) {}

LoggerPlant::~LoggerPlant() = default;

auto LoggerPlant::getInstance() -> LoggerPlant & {
  static LoggerPlant LoggerPlant(LogLevel::DEBUG);
  return LoggerPlant;
}

void LoggerPlant::setLevel(LogLevel level) { current_level_ = level; }

void LoggerPlant::log(const std::string &message) {
  for (int32_t i = 0; i < func_depth_; ++i) {
    log_.append("  ");
  }
  log_.append(message + '\n');
}

void LoggerPlant::output() {
  // std::cout << log_;
  log_.clear();
}

auto LoggerPlant::enterFunc(const std::string &function_name) -> Logger {
  for (int32_t i = 0; i < func_depth_; ++i) {
    log_.append("  ");
  }
  log_.append("Entering function: " + function_name + '\n');
  ++func_depth_;
  return Logger(function_name, this, current_level_);
}

void LoggerPlant::exitFunc(const std::string &function_name) {
  --func_depth_;
  for (int32_t i = 0; i < func_depth_; ++i) {
    log_.append("  ");
  }
  log_.append("Exiting function: " + function_name + '\n');
  LoggerPlant::output();
}

Logger::Logger(const std::string &func_name, LoggerPlant *plant_,
               LogLevel level)
    : func_name_(func_name), plant_(plant_), log_level_(level){};

Logger::~Logger() {
  plant_->log(log_);
  plant_->exitFunc(func_name_);
}

void Logger::log(LogLevel level, const std::string &message) {
  if (level >= log_level_) {
    if (!log_.empty()) {
      log_.append("\n");
    }
    switch (level) {
    case LogLevel::DEBUG:
      log_.append("[DEBUG] ");
      break;
    case LogLevel::INFO:
      log_.append("[INFO] ");
      break;
    case LogLevel::WARN:
      log_.append("[WARN] ");
      break;
    case LogLevel::ERROR:
      log_.append("[ERROR] ");
      break;
    }
    log_.append(message);
  }
}