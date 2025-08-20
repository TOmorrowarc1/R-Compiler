#include "SemanticChecker.hpp"
#include "ASTNodeTotal.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"

SemanticChecker::SemanticChecker(Scope *initial_scope)
    : current_scope_(initial_scope) {}
SemanticChecker::~SemanticChecker() = default;

void SemanticChecker::visit(ASTRootNode *node) {}

void SemanticChecker::visit(ItemConstNode *node) {}
void SemanticChecker::visit(ItemFnNode *node) {}
void SemanticChecker::visit(ItemStructNode *node) {}
void SemanticChecker::visit(ItemEnumNode *node) {}
void SemanticChecker::visit(ItemImplNode *node) {}
void SemanticChecker::visit(ItemTraitNode *node) {}

void SemanticChecker::visit(StmtExprNode *node) {}
void SemanticChecker::visit(StmtEmptyNode *node) {}
void SemanticChecker::visit(StmtItemNode *node) {}
void SemanticChecker::visit(StmtLetNode *node) {}

void SemanticChecker::visit(ExprArrayNode *node) {}
void SemanticChecker::visit(ExprArrayIndexNode *node) {}
void SemanticChecker::visit(ExprBlockNode *node) {}
void SemanticChecker::visit(ExprBlockConstNode *node) {}
void SemanticChecker::visit(ExprCallNode *node) {}
void SemanticChecker::visit(ExprBreakNode *node) {}
void SemanticChecker::visit(ExprReturnNode *node) {}
void SemanticChecker::visit(ExprContinueNode *node) {}
void SemanticChecker::visit(ExprGroupNode *node) {}
void SemanticChecker::visit(ExprIfNode *node) {}
void SemanticChecker::visit(ExprLiteralIntNode *node) {}
void SemanticChecker::visit(ExprLiteralBoolNode *node) {}
void SemanticChecker::visit(ExprLiteralCharNode *node) {}
void SemanticChecker::visit(ExprLiteralStringNode *node) {}
void SemanticChecker::visit(ExprLoopNode *node) {}
void SemanticChecker::visit(ExprWhileNode *node) {}
void SemanticChecker::visit(ExprOperBinaryNode *node) {}
void SemanticChecker::visit(ExprOperUnaryNode *node) {}
void SemanticChecker::visit(ExprPathNode *node) {}
void SemanticChecker::visit(ExprFieldNode *node) {}
void SemanticChecker::visit(ExprMethodNode *node) {}
void SemanticChecker::visit(ExprMatchNode *node) {}
void SemanticChecker::visit(ExprStructNode *node) {}
void SemanticChecker::visit(ExprTupleNode *node) {}
void SemanticChecker::visit(ExprTupleIndexNode *node) {}

void SemanticChecker::visit(PatternLiteralNode *node) {}
void SemanticChecker::visit(PatternStructNode *node) {}
void SemanticChecker::visit(PatternTupleNode *node) {}
void SemanticChecker::visit(PatternWildNode *node) {}
void SemanticChecker::visit(PatternPathNode *node) {}
void SemanticChecker::visit(PatternIDNode *node) {}

void SemanticChecker::visit(TypeArrayNode *node) {}
void SemanticChecker::visit(TypeSliceNode *node) {}
void SemanticChecker::visit(TypeInferNode *node) {}
void SemanticChecker::visit(TypePathNode *node) {}

void SemanticChecker::visit(PathNode *node) {}