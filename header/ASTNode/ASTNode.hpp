#pragma once
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

class Visitor;

class ASTNode {
public:
  ASTNode() = default;
  ~ASTNode() = default;
  virtual void accept(Visitor &visitor) = 0;
};