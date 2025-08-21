#pragma once
#include "ExprANode.hpp"

class ExprBlockNode;

class ExprLoopNode : public ExprBlockInNode {
public:
  std::unique_ptr<ExprBlockNode> loop_body_;

  ExprLoopNode(std::unique_ptr<ExprBlockNode> &&loop_body, Position position);
  ~ExprLoopNode();
  void accept(Visitor &visitor) override;
};

class ExprWhileNode : public ExprBlockInNode {
public:
  std::unique_ptr<ExprNode> condition_;
  std::unique_ptr<ExprBlockNode> loop_body_;

  ExprWhileNode(std::unique_ptr<ExprNode> &&condition,
                std::unique_ptr<ExprBlockNode> &&loop_body, Position position);
  ~ExprWhileNode();
  void accept(Visitor &visitor) override;
};