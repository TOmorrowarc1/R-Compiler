#pragma once
#include "ExprANode.hpp"

class PatternNode;
class Visitor;

struct ExprMatchArm {
  std::unique_ptr<PatternNode> pattern;
  std::unique_ptr<ExprNode> guard;
  std::unique_ptr<ExprNode> body;
};

class ExprMatchNode : public ExprBlockInNode {
private:
  std::unique_ptr<ExprNode> subject_;
  std::vector<ExprMatchArm> arms_;

public:
  ExprMatchNode(std::unique_ptr<ExprNode> &&subject,
                std::vector<ExprMatchArm> &&arms);
  ~ExprMatchNode();
  void accept(Visitor &visitor) override;
};