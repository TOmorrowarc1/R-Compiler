#pragma once
#include "Scope.hpp"
#include "Visitor.hpp"
#include <stack>
/*
There are 2 objects of the third pass after all symbols of types are
collected, members are filled and constants are evaluated:
1. Collect all boundaries(const except) in traits that serves as interfaces
defination in the fourth path for impl blocks.
2. Collect all signatures of functions. no matter they are in or out of types.
*/

class TypeKind;
class TypeNode;
class ConstEvaluator;
enum class ContextType;

class FuncTraitCollector : public Visitor {
private:
  Scope *current_scope_;
  ConstEvaluator *const_evaluator_;
  std::stack<std::string> ctx_name_;
  std::stack<ContextType> context_;

  auto fnNodeToFunc(const ItemFnNode *node)
      -> std::shared_ptr<SymbolFunctionInfo>;

public:
  FuncTraitCollector(Scope *initial_scope, ConstEvaluator *const_evaluator);
  ~FuncTraitCollector() override;

  void visit(ASTRootNode *node) override;

  void visit(ItemConstNode *node) override;
  void visit(ItemFnNode *node) override;
  void visit(ItemStructNode *node) override;
  void visit(ItemEnumNode *node) override;
  void visit(ItemImplNode *node) override;
  void visit(ItemTraitNode *node) override;

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