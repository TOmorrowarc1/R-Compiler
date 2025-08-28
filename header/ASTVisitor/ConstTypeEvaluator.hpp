#pragma once
#include "Scope.hpp"
#include "Visitor.hpp"

/*
The second pass implies types by their members and impls, collects function
signatures and const items.
*/
class TypeKind;
class TypeNode;
class ConstEvaluator;

class ConstTypeEvaluator : public Visitor {
private:
  Scope *current_scope_;
  ConstEvaluator *const_evaluator_;

  auto isStructConst(const PathNode *path_node) -> bool;
  auto getPathIndexName(const PathNode *path_node, uint32_t index)
      -> std::string;
  auto typeNodeToType(const TypeNode *type_node) -> std::shared_ptr<TypeKind>;
  auto fnNodeToFunc(const ItemFnNode *node)
      -> std::shared_ptr<SymbolFunctionInfo>;

public:
  ConstTypeEvaluator(Scope *initial_scope);
  ~ConstTypeEvaluator() override;

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