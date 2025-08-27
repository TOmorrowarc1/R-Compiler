#pragma once
#include "Scope.hpp"
#include "Visitor.hpp"

/*
Traverse a part of the ASTNode tree recursively to calculate value of constants,
supporting basic arithmetic operations and parentheses between numbers. However,
the fowarding defination(decalaration) of const has not been supported yet.
*/

class ConstEvaluator : public Visitor {
private:
  Scope *current_scope_;
  auto getPathIndexName(const PathNode *path_node, uint32_t index)
      -> std::string;
  auto judgeNum(const ExprNode *node) -> bool;

public:
  ConstEvaluator(Scope *initial_scope);
  ~ConstEvaluator() override;

  void visit(ASTRootNode *node) override;

  void visit(ItemConstNode *node) override;
  void visit(ItemFnNode *node) override;
  void visit(ItemStructNode *node) override;
  void visit(ItemEnumNode *node) override;
  void visit(ItemImplNode *node) override;
  void visit(ItemTraitNode *node) override;

  void visit(ExprNode* node);
  void visit(ExprArrayNode *node) override;
  void visit(ExprArrayIndexNode *node) override;
  void visit(ExprBlockNode *node) override;
  void visit(ExprBlockConstNode *node) override;
  void visit(ExprCallNode *node) override;
  void visit(ExprBreakNode *node) override;
  void visit(ExprReturnNode *node) override;
  void visit(ExprContinueNode *node) override;
  void visit(ExprGroupNode *node) override;
  void visit(ExprIfNode *node) override;
  void visit(ExprLiteralIntNode *node) override;
  void visit(ExprLiteralBoolNode *node) override;
  void visit(ExprLiteralCharNode *node) override;
  void visit(ExprLiteralStringNode *node) override;
  void visit(ExprLoopNode *node) override;
  void visit(ExprWhileNode *node) override;
  void visit(ExprOperBinaryNode *node) override;
  void visit(ExprOperUnaryNode *node) override;
  void visit(ExprPathNode *node) override;
  void visit(ExprFieldNode *node) override;
  void visit(ExprMethodNode *node) override;
  void visit(ExprStructNode *node) override;
  void visit(ExprUnderScoreNode *node) override;

  void visit(StmtExprNode *node) override;
  void visit(StmtEmptyNode *node) override;
  void visit(StmtItemNode *node) override;
  void visit(StmtLetNode *node) override;

  void visit(PatternLiteralNode *node) override;
  void visit(PatternWildNode *node) override;
  void visit(PatternPathNode *node) override;
  void visit(PatternIDNode *node) override;
  void visit(PatternReferNode *node) override;

  void visit(TypeArrayNode *node) override;
  void visit(TypePathNode *node) override;
  void visit(TypeReferNode *node) override;

  void visit(PathNode *node) override;
};