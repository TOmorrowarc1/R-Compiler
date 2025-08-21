#pragma once
#include <stdint.h>

class Position {
private:
  uint32_t line;

public:
  Position();
  Position(uint32_t line);
  Position(const Position &other);
  ~Position();

  void setLine(uint32_t newLine);
  auto getLine() const -> uint32_t;
};