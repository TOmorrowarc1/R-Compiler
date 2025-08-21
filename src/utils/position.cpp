#include "position.hpp"

Position::Position() : line(0) {}
Position::Position(uint32_t line) : line(line) {}
Position::Position(const Position &other) : line(other.line) {}
Position::~Position() = default;

void Position::setLine(uint32_t newLine) { line = newLine; }
auto Position::getLine() const -> uint32_t { return line; }