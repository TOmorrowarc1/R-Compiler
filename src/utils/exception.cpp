#include "exception.hpp"

CompilerException::CompilerException(const std::string &message,
                                     Position position)
    : std::runtime_error(message), position_(position) {}

CompilerException::~CompilerException() = default;

auto CompilerException::getExceptionMessage() const -> std::string {
  return "Line " + std::to_string(position_.getLine()) + ": " + what();
}