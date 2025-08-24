#pragma once
#include "Scope.hpp"
#include "Visitor.hpp"

/*
The third pass is semantic check, in which we realize characteristics below:
1. Name Existence: A name must be defined before it is used, including
variables, functions, types, methods, etc.
2. Type Correctness: The type of an expression must match the expected type,
such as assignment, function call, etc.
3. Place Expression: Everything on the lhs of the = should be place expr.
4. Mutability Correctness: A mutable variable must be declared as mutable, and
everything modified is either a mutable var or a mutable reference.
5. Pattern Correctness: Patterns must match the structure of the value they
are matching against when are irrefutable.
6. Control Flow context: Control flow statements like `break`, `continue`, and
`return` must be used in the correct context, such as loops or functions.
7. Const Evaluation: Constants must be evaluated at compile time, and their
values must be known at compile time.
8. Match Patterns: Match arms must cover all possible cases, and patterns must
be valid for the type they are matching against.
9. Recursive Type Definitions: Types can be defined recursively, but must be
well-formed and not lead to infinite recursion.
*/

class TypeKind;
class TypeDef;

class SemanticChecker : public Visitor {
private:
  Scope *current_scope_;
  std::shared_ptr<TypeDef> current_impl_type_;

  auto getPathIndexName(const PathNode *path_node, uint32_t index)
      -> std::string;
  auto typeNodeToType(const TypeNode *type_node) -> std::shared_ptr<TypeKind>;

  auto bindPatternToType(const PatternNode *pattern_node,
                         std::shared_ptr<TypeKind> type) -> bool;

  auto judgeU32(const ExprNode *node) -> bool;
  auto judgeI32(const ExprNode *node) -> bool;
  auto judgeNum(const ExprNode *node) -> bool;

public:
  SemanticChecker(Scope *initial_scope);
  ~SemanticChecker() override;

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