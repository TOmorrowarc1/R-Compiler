#pragma once
#include "ASTNode.hpp"

class PatternNode : public ASTNode {
private:
public:
  PatternNode() : ASTNode(){};
  virtual ~PatternNode() = 0;
  virtual void accept(Visitor &visitor) override = 0;
};

class PatternOneNode : public PatternNode {
private:
public:
  PatternOneNode() : PatternNode(){};
  virtual ~PatternOneNode() = 0;
  virtual void accept(Visitor &visitor) override = 0;
};

class PatternMultNode : public PatternNode {
private:
  std::vector<std::unique_ptr<PatternOneNode>> patterns_;

public:
  PatternMultNode(std::vector<std::unique_ptr<PatternOneNode>> &&patterns_);
  ~PatternMultNode() = default;
  void accept(Visitor &visitor) override;
};