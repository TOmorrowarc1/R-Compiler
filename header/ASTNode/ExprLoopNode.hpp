#pragma once
#include "ExprANode.hpp"

class ExprBlockNode;

class ExprLoopNode : public ExprBlockInNode {
private:
  std::unique_ptr<ExprBlockNode> loop_body_;

public:
  ExprLoopNode(std::unique_ptr<ExprBlockNode> &&loop_body);
  ~ExprLoopNode();
  void accept(Visitor &visitor) override;
};

class ExprWhileNode : public ExprBlockInNode {
private:
  std::unique_ptr<ExprNode> condition_;
  std::unique_ptr<ExprBlockNode> loop_body_;
public:
  ExprWhileNode(std::unique_ptr<ExprNode> &&condition,
                std::unique_ptr<ExprBlockNode> &&loop_body);
  ~ExprWhileNode();
  void accept(Visitor &visitor) override;
};