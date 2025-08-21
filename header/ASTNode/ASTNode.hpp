#pragma once
#include "position.hpp"
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

class Visitor;

class ASTNode {
public:
  Position position_;
  ASTNode(Position position) : position_(position){};
  ~ASTNode() = default;
  virtual void accept(Visitor &visitor) = 0;
};