#pragma once
#include "position.hpp"
#include <stdexcept>

class CompilerException : public std::runtime_error {
private:
  Position position_;

public:
  CompilerException(const std::string &message, Position position);
  ~CompilerException();
  auto getExceptionMessage() const -> std::string;
};
